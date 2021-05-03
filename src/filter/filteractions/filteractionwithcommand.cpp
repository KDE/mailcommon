/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionwithcommand.h"
#include "mailcommon_debug.h"
#include <KProcess>
#include <KShell>

#include <QRegularExpression>
#include <QTemporaryFile>

using namespace MailCommon;

FilterActionWithCommand::FilterActionWithCommand(const QString &name, const QString &label, QObject *parent)
    : FilterActionWithUrl(name, label, parent)
{
}

QWidget *FilterActionWithCommand::createParamWidget(QWidget *parent) const
{
    return FilterActionWithUrl::createParamWidget(parent);
}

void FilterActionWithCommand::applyParamWidgetValue(QWidget *paramWidget)
{
    FilterActionWithUrl::applyParamWidgetValue(paramWidget);
}

void FilterActionWithCommand::setParamWidgetValue(QWidget *paramWidget) const
{
    FilterActionWithUrl::setParamWidgetValue(paramWidget);
}

void FilterActionWithCommand::clearParamWidget(QWidget *paramWidget) const
{
    FilterActionWithUrl::clearParamWidget(paramWidget);
}

static KMime::Content *findMimeNodeForIndex(KMime::Content *node, int &index)
{
    if (index <= 0) {
        return node;
    }

    const QVector<KMime::Content *> lstContents = node->contents();
    for (KMime::Content *child : lstContents) {
        KMime::Content *result = findMimeNodeForIndex(child, --index);
        if (result) {
            return result;
        }
    }

    return nullptr;
}

QString FilterActionWithCommand::substituteCommandLineArgsFor(const KMime::Message::Ptr &aMsg, QVector<QTemporaryFile *> &aTempFileList) const
{
    QString result = mParameter;
    QList<int> argList;
    const QRegularExpression re(QStringLiteral("%([0-9-]+)"));

    // search for '%n'
    QRegularExpressionMatchIterator iter = re.globalMatch(result);
    while (iter.hasNext()) {
        // and save the encountered 'n' in a list.
        bool ok = false;
        const int n = iter.next().captured(1).toInt(&ok);
        if (ok) {
            argList.append(n);
        }
    }

    // sort the list of n's
    std::sort(argList.begin(), argList.end());

    // and use QString::arg to substitute filenames for the %n's.
    int lastSeen = -2;
    QString tempFileName;
    QList<int>::ConstIterator end(argList.constEnd());
    for (QList<int>::ConstIterator it = argList.constBegin(); it != end; ++it) {
        // setup temp files with check for duplicate %n's
        if ((*it) != lastSeen) {
            auto tempFile = new QTemporaryFile();
            if (!tempFile->open()) {
                delete tempFile;
                qCDebug(MAILCOMMON_LOG) << "FilterActionWithCommand: Could not create temp file!";
                return QString();
            }

            aTempFileList.append(tempFile);
            tempFileName = tempFile->fileName();

            QFile file(tempFileName);
            if (!file.open(QIODevice::WriteOnly)) {
                qCWarning(MAILCOMMON_LOG) << "Failed to write message to file: " << file.errorString();
                tempFile->close();
                continue;
            }

            if ((*it) == -1) {
                file.write(aMsg->encodedContent());
            } else if (aMsg->contents().isEmpty()) {
                file.write(aMsg->decodedContent());
            } else {
                int index = *it; // we pass by reference below, so this is not const
                KMime::Content *content = findMimeNodeForIndex(aMsg.data(), index);
                if (content) {
                    file.write(content->decodedContent());
                }
            }
            file.close();
            tempFile->close();
        }

        // QString( "%0 and %1 and %1" ).arg( 0 ).arg( 1 )
        // returns "0 and 1 and %1", so we must call .arg as
        // many times as there are %n's, regardless of their multiplicity.
        if ((*it) == -1) {
            result.replace(QLatin1String("%-1"), tempFileName);
        } else {
            result = result.arg(tempFileName);
        }
    }

    return result;
}

namespace
{
/**
 * Substitutes placeholders in the command line with the
 * content of the corresponding header in the message.
 * %{From} -> Joe Author <joe@acme.com>
 */
void substituteMessageHeaders(const KMime::Message::Ptr &aMsg, QString &result)
{
    // Replace the %{foo} with the content of the foo header field.
    // If the header doesn't exist, remove the placeholder.
    const QRegularExpression header_rx(QStringLiteral("%\\{([a-z0-9-]+)\\}"), QRegularExpression::CaseInsensitiveOption);
    int offset = 0;
    QRegularExpressionMatch rmatch;
    while (result.indexOf(header_rx, offset, &rmatch) != -1) {
        const KMime::Headers::Base *header = aMsg->headerByType(rmatch.captured(1).toLatin1().constData());
        QString replacement;
        if (header) {
            replacement = KShell::quoteArg(QString::fromLatin1(header->as7BitString()));
        }
        const int start = rmatch.capturedStart(0);
        result.replace(start, rmatch.capturedLength(0), replacement);
        offset = start + replacement.size();
    }
}

/**
 * Substitutes placeholders in the command line with the
 * corresponding information from the item. Currently supported
 * are %{itemid} and %{itemurl}.
 */
void substituteCommandLineArgsForItem(const Akonadi::Item &item, QString &commandLine)
{
    commandLine.replace(QLatin1String("%{itemurl}"), item.url(Akonadi::Item::UrlWithMimeType).url());
    commandLine.replace(QLatin1String("%{itemid}"), QString::number(item.id()));
}
}

FilterAction::ReturnCode FilterActionWithCommand::genericProcess(ItemContext &context, bool withOutput) const
{
    const auto aMsg = context.item().payload<KMime::Message::Ptr>();
    Q_ASSERT(aMsg);

    if (mParameter.isEmpty()) {
        return ErrorButGoOn;
    }

    // KProcess doesn't support a QProcess::launch() equivalent, so
    // we must use a temp file :-(
    auto inFile = new QTemporaryFile;
    if (!inFile->open()) {
        delete inFile;
        return ErrorButGoOn;
    }

    QVector<QTemporaryFile *> atmList;
    atmList.append(inFile);

    QString commandLine = substituteCommandLineArgsFor(aMsg, atmList);
    substituteCommandLineArgsForItem(context.item(), commandLine);
    substituteMessageHeaders(aMsg, commandLine);

    if (commandLine.isEmpty()) {
        qDeleteAll(atmList);
        atmList.clear();
        return ErrorButGoOn;
    }
    // The parentheses force the creation of a subshell
    // in which the user-specified command is executed.
    // This is to really catch all output of the command as well
    // as to avoid clashes of our redirection with the ones
    // the user may have specified. In the long run, we
    // shouldn't be using tempfiles at all for this class, due
    // to security aspects. (mmutz)
    commandLine = QLatin1Char('(') + commandLine + QLatin1String(") <") + inFile->fileName();

    // write message to file
    QString tempFileName = inFile->fileName();
    QFile tempFile(tempFileName);
    if (!tempFile.open(QIODevice::ReadWrite)) {
        qCWarning(MAILCOMMON_LOG) << "Failed to write message to file: " << tempFile.errorString();
        qDeleteAll(atmList);
        atmList.clear();
        return CriticalError;
    }
    tempFile.write(aMsg->encodedContent());
    tempFile.close();
    inFile->close();

    KProcess shProc;
    shProc.setOutputChannelMode(KProcess::SeparateChannels);
    shProc.setShellCommand(commandLine);
    int result = shProc.execute();

    if (result != 0) {
        qDeleteAll(atmList);
        atmList.clear();
        return ErrorButGoOn;
    }

    if (withOutput) {
        // read altered message:
        const QByteArray msgText = shProc.readAllStandardOutput();

        if (!msgText.trimmed().isEmpty()) {
            /* If the pipe through alters the message, it could very well
            happen that it no longer has a X-UID header afterwards. That is
            unfortunate, as we need to removed the original from the folder
            using that, and look it up in the message. When the (new) message
            is uploaded, the header is stripped anyhow. */
            QString uid;
            if (auto hrd = aMsg->headerByType("X-UID")) {
                uid = hrd->asUnicodeString();
            }
            aMsg->setContent(KMime::CRLFtoLF(msgText));
            aMsg->setFrozen(true);
            aMsg->parse();

            QString newUid;
            if (auto hrd = aMsg->headerByType("X-UID")) {
                newUid = hrd->asUnicodeString();
            }
            if (uid != newUid) {
                aMsg->setFrozen(false);
                auto header = new KMime::Headers::Generic("X-UID");
                header->fromUnicodeString(uid, "utf-8");
                aMsg->setHeader(header);
                aMsg->assemble();
            }

            context.setNeedsPayloadStore();
        } else {
            qDeleteAll(atmList);
            atmList.clear();
            return ErrorButGoOn;
        }
    }

    qDeleteAll(atmList);
    atmList.clear();

    return GoOn;
}

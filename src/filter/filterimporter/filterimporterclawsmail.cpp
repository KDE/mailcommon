/*
  SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filterimporterclawsmail.h"
using namespace Qt::Literals::StringLiterals;

#include "filter/mailfilter.h"
#include "mailcommon_debug.h"

#include <QDir>
#include <QFile>

using namespace MailCommon;

FilterImporterClawsMails::FilterImporterClawsMails(QFile *file)

{
    QTextStream stream(file);
    readStream(stream);
}

FilterImporterClawsMails::FilterImporterClawsMails(QString string)

{
    QTextStream stream(&string);
    readStream(stream);
}

FilterImporterClawsMails::FilterImporterClawsMails(bool interactive)
    : FilterImporterAbstract(interactive)
{
}

FilterImporterClawsMails::~FilterImporterClawsMails() = default;

void FilterImporterClawsMails::readStream(QTextStream &stream)
{
    MailFilter *filter = nullptr;
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        qCDebug(MAILCOMMON_LOG) << " line :" << line << " filter " << filter;

        if (line.isEmpty()) {
            // Nothing
        } else if (line.startsWith(u'[') && line.endsWith(u']')) {
            // TODO
        } else {
            appendFilter(filter);
            filter = parseLine(line);
        }
    }
    appendFilter(filter);
}

QString FilterImporterClawsMails::defaultFiltersSettingsPath()
{
    return QStringLiteral("%1/.claws-mail/matcherrc").arg(QDir::homePath());
}

MailFilter *FilterImporterClawsMails::parseLine(const QString &line)
{
    auto filter = new MailFilter();
    QString tmp = line;
    // Enabled ?
    if (tmp.startsWith(QLatin1StringView("enabled"))) {
        filter->setEnabled(true);
        tmp.remove(QStringLiteral("enabled "));
    }

    // Filter name
    if (tmp.startsWith(QLatin1StringView("rulename"))) {
        tmp.remove(QStringLiteral("rulename "));
        int pos;
        const QString name = extractString(tmp, pos);
        filter->pattern()->setName(name);
        filter->setToolbarName(name);

        tmp = tmp.mid(pos + 2); // remove "\" "
        qCDebug(MAILCOMMON_LOG) << " new tmp" << tmp;
    }

    tmp = extractConditions(tmp, filter);

    tmp = extractActions(tmp, filter);
    // TODO
    return filter;
}

QString FilterImporterClawsMails::extractActions(const QString &line, MailFilter *filter)
{
    Q_UNUSED(filter)
    return line;
}

QString FilterImporterClawsMails::extractConditions(const QString &line, MailFilter *filter)
{
    QByteArray fieldName;
    // Action
    if (line.startsWith(QLatin1StringView("subject"))) {
        fieldName = "subject";
    } else if (line.startsWith(QLatin1StringView("age_lower"))) {
    }
    filter->pattern()->setOp(SearchPattern::OpAnd);
    // TODO
    return {};
}

QString FilterImporterClawsMails::extractString(const QString &tmp, int &pos)
{
    QString name;
    QChar previousChar;
    int i = 0;
    for (; i < tmp.length(); ++i) {
        const QChar currentChar = tmp.at(i);
        if (i == 0 && (currentChar.isSpace() || currentChar == u'"')) {
        } else {
            if (currentChar != u'"') {
                if (currentChar != u'\\') {
                    name += currentChar;
                }
            } else {
                if (previousChar == u'\\') {
                    name += currentChar;
                } else {
                    break;
                }
            }
            previousChar = currentChar;
        }
    }
    pos = i;
    qCDebug(MAILCOMMON_LOG) << " name " << name;
    return name;
}

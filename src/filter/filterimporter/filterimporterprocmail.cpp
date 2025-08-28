/*
  SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filterimporterprocmail.h"
using namespace Qt::Literals::StringLiterals;

#include "filter/mailfilter.h"
#include "mailcommon_debug.h"
#include <KLocalizedString>

#include <QDir>
#include <QFile>

using namespace MailCommon;

FilterImporterProcmail::FilterImporterProcmail(QFile *file)

{
    QTextStream stream(file);
    readStream(stream);
}

FilterImporterProcmail::FilterImporterProcmail(QString string)

{
    QTextStream stream(&string);
    readStream(stream);
}

FilterImporterProcmail::~FilterImporterProcmail() = default;

void FilterImporterProcmail::readStream(QTextStream &stream)
{
    MailFilter *filter = nullptr;
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        qCDebug(MAILCOMMON_LOG) << " line :" << line << " filter " << filter;
        filter = parseLine(stream, line, filter);
    }

    appendFilter(filter);
}

QString FilterImporterProcmail::defaultFiltersSettingsPath()
{
    return QDir::homePath();
}

QString FilterImporterProcmail::createUniqFilterName()
{
    return i18n("Procmail filter %1", ++mFilterCount);
}

MailCommon::MailFilter *FilterImporterProcmail::parseLine(QTextStream &stream, QString line, MailCommon::MailFilter *filter)
{
    Q_UNUSED(stream)
    if (line.isEmpty()) {
        // Empty line
        return filter;
    } else if (line.startsWith(u'#')) {
        // Commented line
        return filter;
    } else if (line.startsWith(QLatin1StringView(":0"))) {
        appendFilter(filter);
        filter = new MailFilter();
        const QString uniqName = createUniqFilterName();
        filter->pattern()->setName(uniqName);
        filter->setToolbarName(uniqName);
    } else if (line.startsWith(QLatin1StringView("* "))) {
        line.remove(0, 2);
        QByteArray fieldName;
        SearchRule::Function functionName = SearchRule::FuncRegExp;
        if (line.startsWith(QLatin1StringView("^From:"))) {
            line.remove(QStringLiteral("^From:"));
            fieldName = "from";
        } else if (line.startsWith(QLatin1StringView("^Subject:"))) {
            line.remove(QStringLiteral("^Subject:"));
            fieldName = "subject";
        } else if (line.startsWith(QLatin1StringView("^Sender:"))) {
            line.remove(QStringLiteral("^Sender:"));
        } else if (line.startsWith(QLatin1StringView("^(To|Cc):"))) {
            line.remove(QStringLiteral("^(To|Cc):"));
            fieldName = "<recipients>";
        } else {
            qCDebug(MAILCOMMON_LOG) << " line condition not parsed :" << line;
        }
        SearchRule::Ptr rule = SearchRule::createInstance(fieldName, functionName, line);
        filter->pattern()->append(rule);
        // Condition
    } else if (line.startsWith(u'!')) {
        line.remove(u'!');
        // Redirect email
    } else if (line.startsWith(u'|')) {
        // Shell
        const QString actionName(QStringLiteral("execute"));
        const QString value(line);
        createFilterAction(filter, actionName, value);
    } else if (line.startsWith(u'{')) {
        // Block
    } else if (line.startsWith(u'}')) {
        // End block
    } else {
        const QString actionName(QStringLiteral("transfer"));
        const QString value(line);
        createFilterAction(filter, actionName, value);
        // Folder
    }

    return filter;
}

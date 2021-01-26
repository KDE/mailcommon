/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filterimporterprocmail.h"
#include "filter/filtermanager.h"
#include "filter/mailfilter.h"
#include "mailcommon_debug.h"
#include <KLocalizedString>

#include <QDir>
#include <QFile>

using namespace MailCommon;

FilterImporterProcmail::FilterImporterProcmail(QFile *file)
    : FilterImporterAbstract()
{
    QTextStream stream(file);
    readStream(stream);
}

FilterImporterProcmail::FilterImporterProcmail(QString string)
    : FilterImporterAbstract()
{
    QTextStream stream(&string);
    readStream(stream);
}

FilterImporterProcmail::~FilterImporterProcmail()
{
}

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
    } else if (line.startsWith(QLatin1Char('#'))) {
        // Commented line
        return filter;
    } else if (line.startsWith(QLatin1String(":0"))) {
        appendFilter(filter);
        filter = new MailFilter();
        const QString uniqName = createUniqFilterName();
        filter->pattern()->setName(uniqName);
        filter->setToolbarName(uniqName);
    } else if (line.startsWith(QLatin1String("* "))) {
        line.remove(0, 2);
        QByteArray fieldName;
        SearchRule::Function functionName = SearchRule::FuncRegExp;
        if (line.startsWith(QLatin1String("^From:"))) {
            line.remove(QStringLiteral("^From:"));
            fieldName = "from";
        } else if (line.startsWith(QLatin1String("^Subject:"))) {
            line.remove(QStringLiteral("^Subject:"));
            fieldName = "subject";
        } else if (line.startsWith(QLatin1String("^Sender:"))) {
            line.remove(QStringLiteral("^Sender:"));
        } else if (line.startsWith(QLatin1String("^(To|Cc):"))) {
            line.remove(QStringLiteral("^(To|Cc):"));
            fieldName = "<recipients>";
        } else {
            qCDebug(MAILCOMMON_LOG) << " line condition not parsed :" << line;
        }
        SearchRule::Ptr rule = SearchRule::createInstance(fieldName, functionName, line);
        filter->pattern()->append(rule);
        // Condition
    } else if (line.startsWith(QLatin1Char('!'))) {
        line.remove(QLatin1Char('!'));
        // Redirect email
    } else if (line.startsWith(QLatin1Char('|'))) {
        // Shell
        const QString actionName(QStringLiteral("execute"));
        const QString value(line);
        createFilterAction(filter, actionName, value);
    } else if (line.startsWith(QLatin1Char('{'))) {
        // Block
    } else if (line.startsWith(QLatin1Char('}'))) {
        // End block
    } else {
        const QString actionName(QStringLiteral("transfer"));
        const QString value(line);
        createFilterAction(filter, actionName, value);
        // Folder
    }

    return filter;
}

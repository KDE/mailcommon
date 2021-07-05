/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filterimporterbalsa.h"
#include "filter/filtermanager.h"
#include "filter/mailfilter.h"
#include "mailcommon_debug.h"

#include <KConfig>
#include <KConfigGroup>

#include <QDir>
#include <QFile>
#include <QRegularExpression>

using namespace MailCommon;

FilterImporterBalsa::FilterImporterBalsa(QFile *file)
    : FilterImporterAbstract()
{
    KConfig config(file->fileName());
    readConfig(&config);
}

FilterImporterBalsa::FilterImporterBalsa()
    : FilterImporterAbstract()
{
}

FilterImporterBalsa::~FilterImporterBalsa()
{
}

QString FilterImporterBalsa::defaultFiltersSettingsPath()
{
    return QStringLiteral("%1/.balsa/config").arg(QDir::homePath());
}

void FilterImporterBalsa::readConfig(KConfig *config)
{
    const QStringList filterList = config->groupList().filter(QRegularExpression(QStringLiteral("filter-\\d+")));
    for (const QString &filter : filterList) {
        KConfigGroup grp = config->group(filter);
        parseFilter(grp);
    }
}

void FilterImporterBalsa::parseFilter(const KConfigGroup &grp)
{
    auto filter = new MailCommon::MailFilter();
    const QString name = grp.readEntry(QStringLiteral("Name"));
    filter->pattern()->setName(name);
    filter->setToolbarName(name);

    // TODO not implemented in kmail.
    // const QString popupText = grp.readEntry(QStringLiteral("Popup-text"));

    const QString sound = grp.readEntry(QStringLiteral("Sound"));
    if (!sound.isEmpty()) {
        const QString actionName = QStringLiteral("play sound");
        createFilterAction(filter, actionName, sound);
    }

    const int actionType = grp.readEntry(QStringLiteral("Action-type"), -1);
    const QString actionStr = grp.readEntry(QStringLiteral("Action-string"));
    parseAction(actionType, actionStr, filter);

    const QString condition = grp.readEntry(QStringLiteral("Condition"));
    parseCondition(condition, filter);

    appendFilter(filter);
}

void FilterImporterBalsa::parseCondition(const QString &condition, MailCommon::MailFilter *filter)
{
    QStringList conditionList;
    if (condition.startsWith(QLatin1String("OR "))) {
        conditionList = condition.split(QStringLiteral("OR"));
        filter->pattern()->setOp(SearchPattern::OpOr);
    } else if (condition.startsWith(QLatin1String("AND "))) {
        conditionList = condition.split(QStringLiteral("AND"));
        filter->pattern()->setOp(SearchPattern::OpAnd);
    } else {
        // no multi condition
        conditionList << condition;
    }
    for (QString cond : std::as_const(conditionList)) {
        cond = cond.trimmed();
        if (cond.startsWith(QLatin1String("NOT"))) {
            cond = cond.right(cond.length() - 3);
            cond = cond.trimmed();
        }
        qCDebug(MAILCOMMON_LOG) << " cond" << cond;

        // Date between
        QByteArray fieldName;
        if (cond.startsWith(QLatin1String("DATE"))) {
            fieldName = "<date>";
            cond = cond.right(cond.length() - 4);
            cond = cond.trimmed();
            QStringList splitDate = cond.split(QLatin1Char(' '));
            qCDebug(MAILCOMMON_LOG) << " splitDate " << splitDate;
        } else if (cond.startsWith(QLatin1String("FLAG"))) {
            qCDebug(MAILCOMMON_LOG) << " FLAG :";
        } else if (cond.startsWith(QLatin1String("STRING"))) {
            qCDebug(MAILCOMMON_LOG) << " STRING";
        } else {
            qCDebug(MAILCOMMON_LOG) << " condition not implemented :" << cond;
        }

        // SearchRule::Ptr rule = SearchRule::createInstance( fieldName, functionName, line );
        // filter->pattern()->append( rule );
    }
}

void FilterImporterBalsa::parseAction(int actionType, const QString &action, MailCommon::MailFilter *filter)
{
    QString actionName;
    QString actionStr(action);
    switch (actionType) {
    case 0:
        break;
    case 1:
        // Copy
        actionName = QStringLiteral("copy");
        break;
    case 2:
        // Move
        actionName = QStringLiteral("transfer");
        break;
    case 3:
        // Print
        // Not implemented in kmail
        break;
    case 4:
        // Execute
        actionName = QStringLiteral("execute");
        break;
    case 5:
        // Move to trash
        actionName = QStringLiteral("transfer");
        // Special !
        break;
    case 6:
        // Put color
        break;
    default:
        qCDebug(MAILCOMMON_LOG) << " unknown parse action type " << actionType;
        break;
    }
    if (!actionName.isEmpty()) {
        // TODO adapt actionStr
        createFilterAction(filter, actionName, actionStr);
    }
}

/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filterimportergmail.h"
#include "filter/mailfilter.h"
#include "mailcommon_debug.h"
#include <QDir>
#include <QDomDocument>
#include <QFile>

using namespace MailCommon;

FilterImporterGmail::FilterImporterGmail(QFile *file)
    : FilterImporterAbstract()
{
    QDomDocument doc;
    if (!loadDomElement(doc, file)) {
        return;
    }

    QDomElement filters = doc.documentElement();

    if (filters.isNull()) {
        qCDebug(MAILCOMMON_LOG) << "No filters defined";
        return;
    }
    for (QDomElement e = filters.firstChildElement(); !e.isNull(); e = e.nextSiblingElement()) {
        const QString tag = e.tagName();
        if (tag == QLatin1String("entry")) {
            qCDebug(MAILCOMMON_LOG) << " filter found !";
            parseFilters(e);
        }
    }
}

FilterImporterGmail::~FilterImporterGmail()
{
}

QString FilterImporterGmail::defaultFiltersSettingsPath()
{
    return QDir::homePath();
}

QString FilterImporterGmail::createUniqFilterName()
{
    return i18n("Gmail filter %1", ++mFilterCount);
}

void FilterImporterGmail::parseFilters(const QDomElement &e)
{
    auto filter = new MailCommon::MailFilter();
    filter->setAutoNaming(true);
    const QString uniqName = createUniqFilterName();
    filter->pattern()->setName(uniqName);
    filter->setToolbarName(uniqName);
    filter->setEnabled(true);
    QByteArray fieldName;
    for (QDomElement ruleFilter = e.firstChildElement(); !ruleFilter.isNull(); ruleFilter = ruleFilter.nextSiblingElement()) {
        const QString tagName = ruleFilter.tagName();
        if (tagName == QLatin1String("category")) {
            if (ruleFilter.hasAttribute(QStringLiteral("term"))) {
                if (ruleFilter.attribute(QStringLiteral("term")) != QLatin1String("filter")) {
                    continue;
                }
            }
        } else if (tagName == QLatin1String("apps:property")) {
            if (ruleFilter.hasAttribute(QStringLiteral("name"))) {
                const QString criteriaProperty = ruleFilter.attribute(QStringLiteral("name"));
                qCDebug(MAILCOMMON_LOG) << " ruleFilter.attribute" << criteriaProperty;
                // Criterial
                if (criteriaProperty == QLatin1String("from")) {
                    fieldName = "from";
                } else if (criteriaProperty == QLatin1String("to")) {
                    fieldName = "to";
                } else if (criteriaProperty == QLatin1String("subject")) {
                    fieldName = "subject";
                } else if (criteriaProperty == QLatin1String("hasTheWord")) {
                } else if (criteriaProperty == QLatin1String("doesNotHaveTheWord")) {
                } else if (criteriaProperty == QLatin1String("hasAttachment")) {
                    fieldName = "<size>";
                }
                // Action
                else if (criteriaProperty == QLatin1String("shouldArchive")) {
                } else if (criteriaProperty == QLatin1String("shouldMarkAsRead")) {
                } else if (criteriaProperty == QLatin1String("shouldStar")) {
                } else if (criteriaProperty == QLatin1String("label")) {
                } else if (criteriaProperty == QLatin1String("forwardTo")) {
                } else if (criteriaProperty == QLatin1String("shouldTrash")) {
                } else if (criteriaProperty == QLatin1String("neverSpam")) {
                } else {
                    qCDebug(MAILCOMMON_LOG) << " unknown item " << criteriaProperty;
                }
            }
        }
    }
    appendFilter(filter);
}

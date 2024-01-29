/*
  SPDX-FileCopyrightText: 2012-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filterimportersylpheed.h"
#include "filter/mailfilter.h"
#include "mailcommon_debug.h"

#include <QDir>
#include <QFile>

using namespace MailCommon;

FilterImporterSylpheed::FilterImporterSylpheed(QFile *file)
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
        if (tag == QLatin1StringView("rule")) {
            parseFilters(e);
        } else {
            qCDebug(MAILCOMMON_LOG) << " unknown tag " << tag;
        }
    }
}

FilterImporterSylpheed::~FilterImporterSylpheed() = default;

QString FilterImporterSylpheed::defaultFiltersSettingsPath()
{
    return QStringLiteral("%1/.sylpheed-2.0/filter.xml").arg(QDir::homePath());
}

void FilterImporterSylpheed::parseConditions(const QDomElement &e, MailCommon::MailFilter *filter)
{
    if (e.hasAttribute(QStringLiteral("bool"))) {
        const QString attr = e.attribute(QStringLiteral("bool"));
        if (attr == QLatin1StringView("and")) {
            filter->pattern()->setOp(SearchPattern::OpAnd);
        } else if (attr == QLatin1StringView("or")) {
            filter->pattern()->setOp(SearchPattern::OpOr);
        } else {
            qCDebug(MAILCOMMON_LOG) << " bool not defined: " << attr;
        }
    }
    for (QDomElement ruleFilter = e.firstChildElement(); !ruleFilter.isNull(); ruleFilter = ruleFilter.nextSiblingElement()) {
        QString contentsName;
        QByteArray fieldName;
        SearchRule::Function functionName = SearchRule::FuncNone;

        const QString nexttag = ruleFilter.tagName();
        if (nexttag == QLatin1StringView("match-header")) {
            if (ruleFilter.hasAttribute(QStringLiteral("name"))) {
                const QString attr = ruleFilter.attribute(QStringLiteral("name"));
                if (attr == QLatin1StringView("From")) {
                    fieldName = "from";
                } else if (attr == QLatin1StringView("Cc")) {
                    fieldName = "cc";
                } else if (attr == QLatin1StringView("To")) {
                    fieldName = "to";
                } else if (attr == QLatin1StringView("Reply-To")) {
                    fieldName = "reply-to";
                } else if (attr == QLatin1StringView("Subject")) {
                    fieldName = "subject";
                } else if (attr == QLatin1StringView("List-Id")) {
                    fieldName = "list-id";
                } else if (attr == QLatin1StringView("X-ML-Name")) {
                    fieldName = "x-mailing-list";
                }
                if (fieldName.isEmpty()) {
                    qCDebug(MAILCOMMON_LOG) << " match-header not implemented " << attr;
                }
            }
            contentsName = ruleFilter.text();
        } else if (nexttag == QLatin1StringView("match-any-header")) {
            fieldName = "<any header>";
            contentsName = ruleFilter.text();
        } else if (nexttag == QLatin1StringView("match-to-or-cc")) {
            fieldName = "<recipients>";
            contentsName = ruleFilter.text();
        } else if (nexttag == QLatin1StringView("match-body-text")) {
            fieldName = "<body>";
            contentsName = ruleFilter.text();
        } else if (nexttag == QLatin1StringView("command-test")) {
            // TODO
            // Not implemented in kmail
        } else if (nexttag == QLatin1StringView("size")) {
            fieldName = "<size>";
            contentsName = QString::number(ruleFilter.text().toInt() * 1024); // Stored as kb
        } else if (nexttag == QLatin1StringView("age")) {
            fieldName = "<age in days>";
            contentsName = ruleFilter.text();
        } else if (nexttag == QLatin1StringView("unread")) {
            fieldName = "<status>";
            contentsName = QStringLiteral("Unread");
        } else if (nexttag == QLatin1StringView("mark")) {
            // TODO
        } else if (nexttag == QLatin1StringView("color-label")) {
            // TODO
        } else if (nexttag == QLatin1StringView("mime")) {
            // TODO
        } else if (nexttag == QLatin1StringView("account-id")) {
            // TODO
        } else if (nexttag == QLatin1StringView("target-folder")) {
            // TODO
        } else {
            qCDebug(MAILCOMMON_LOG) << " tag not recognize " << nexttag;
        }
        if (fieldName.isEmpty()) {
            qCDebug(MAILCOMMON_LOG) << " field not implemented " << nexttag;
        }

        if (ruleFilter.hasAttribute(QStringLiteral("type"))) {
            const QString attr = ruleFilter.attribute(QStringLiteral("type"));
            if (attr == QLatin1StringView("not-contain")) {
                functionName = SearchRule::FuncContainsNot;
            } else if (attr == QLatin1StringView("contains")) {
                functionName = SearchRule::FuncContains;
            } else if (attr == QLatin1StringView("is-not")) {
                functionName = SearchRule::FuncNotEqual;
            } else if (attr == QLatin1StringView("is")) {
                functionName = SearchRule::FuncEquals;
            } else if (attr == QLatin1StringView("not-regex")) {
                functionName = SearchRule::FuncNotRegExp;
            } else if (attr == QLatin1StringView("regex")) {
                functionName = SearchRule::FuncRegExp;
            } else if (attr == QLatin1StringView("not-in-addressbook")) {
                functionName = SearchRule::FuncIsNotInAddressbook;
            } else if (attr == QLatin1StringView("in-addressbook")) {
                functionName = SearchRule::FuncIsInAddressbook;
            } else if (attr == QLatin1StringView("gt")) {
                functionName = SearchRule::FuncIsGreater;
            } else if (attr == QLatin1StringView("lt")) {
                functionName = SearchRule::FuncIsLess;
            } else {
                qCDebug(MAILCOMMON_LOG) << " Attr type not implemented :" << attr;
            }
        }
        SearchRule::Ptr rule = SearchRule::createInstance(fieldName, functionName, contentsName);
        filter->pattern()->append(rule);
    }
}

void FilterImporterSylpheed::parseActions(const QDomElement &e, MailCommon::MailFilter *filter)
{
    for (QDomElement ruleFilter = e.firstChildElement(); !ruleFilter.isNull(); ruleFilter = ruleFilter.nextSiblingElement()) {
        QString actionName;
        const QString nexttag = ruleFilter.tagName();
        QString value = ruleFilter.text();
        if (nexttag == QLatin1StringView("move")) {
            actionName = QStringLiteral("transfer");
            value = ruleFilter.text();
        } else if (nexttag == QLatin1StringView("copy")) {
            actionName = QStringLiteral("copy");
            value = ruleFilter.text();
        } else if (nexttag == QLatin1StringView("not-receive")) {
            // TODO
        } else if (nexttag == QLatin1StringView("delete")) {
            actionName = QStringLiteral("delete");
        } else if (nexttag == QLatin1StringView("exec")) {
            actionName = QStringLiteral("execute");
            value = ruleFilter.text();
        } else if (nexttag == QLatin1StringView("exec-async")) {
            actionName = QStringLiteral("filter app");
            value = ruleFilter.text();
        } else if (nexttag == QLatin1StringView("mark")) {
            // FIXME add tag ?
        } else if (nexttag == QLatin1StringView("color-label")) {
            // TODO
        } else if (nexttag == QLatin1StringView("mark-as-read")) {
            actionName = QStringLiteral("set status");
            value = QStringLiteral("R");
        } else if (nexttag == QLatin1StringView("forward")) {
            actionName = QStringLiteral("forward");
            value = ruleFilter.text();
        } else if (nexttag == QLatin1StringView("forward-as-attachment")) {
            // TODO
        } else if (nexttag == QLatin1StringView("redirect")) {
            actionName = QStringLiteral("redirect");
            value = ruleFilter.text();
        } else if (nexttag == QLatin1StringView("stop-eval")) {
            filter->setStopProcessingHere(true);
            break;
        }

        if (actionName.isEmpty()) {
            qCDebug(MAILCOMMON_LOG) << " tag not recognize " << nexttag;
        }
        createFilterAction(filter, actionName, value);
    }
}

void FilterImporterSylpheed::parseFilters(const QDomElement &e)
{
    auto filter = new MailCommon::MailFilter();
    if (e.hasAttribute(QStringLiteral("enabled"))) {
        const QString attr = e.attribute(QStringLiteral("enabled"));
        if (attr == QLatin1StringView("false")) {
            filter->setEnabled(false);
        }
    }

    if (e.hasAttribute(QStringLiteral("name"))) {
        const QString attr = e.attribute(QStringLiteral("name"));
        filter->pattern()->setName(attr);
        filter->setToolbarName(attr);
    }

    if (e.hasAttribute(QStringLiteral("timing"))) {
        const QString attr = e.attribute(QStringLiteral("timing"));
        if (attr == QLatin1StringView("any")) {
            filter->setApplyOnInbound(true);
            filter->setApplyOnExplicit(true);
        } else if (attr == QLatin1StringView("receiver")) {
            filter->setApplyOnInbound(true);
        } else if (attr == QLatin1StringView("manual")) {
            filter->setApplyOnInbound(false);
            filter->setApplyOnExplicit(true);
        } else {
            qCDebug(MAILCOMMON_LOG) << " timing not defined: " << attr;
        }
    }
    for (QDomElement ruleFilter = e.firstChildElement(); !ruleFilter.isNull(); ruleFilter = ruleFilter.nextSiblingElement()) {
        const QString nexttag = ruleFilter.tagName();
        if (nexttag == QLatin1StringView("condition-list")) {
            parseConditions(ruleFilter, filter);
        } else if (nexttag == QLatin1StringView("action-list")) {
            parseActions(ruleFilter, filter);
        } else {
            qCDebug(MAILCOMMON_LOG) << " next tag not implemented " << nexttag;
        }
    }

    appendFilter(filter);
}

/*
  SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filterimporterevolution.h"
#include "filter/mailfilter.h"

#include "mailcommon_debug.h"
#include <QDir>
#include <QFile>
using namespace MailCommon;

FilterImporterEvolution::FilterImporterEvolution(QFile *file)

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
    filters = filters.firstChildElement(QStringLiteral("ruleset"));
    for (QDomElement e = filters.firstChildElement(); !e.isNull(); e = e.nextSiblingElement()) {
        const QString tag = e.tagName();
        if (tag == QLatin1StringView("rule")) {
            parseFilters(e);
        } else {
            qCDebug(MAILCOMMON_LOG) << " unknown tag " << tag;
        }
    }
}

FilterImporterEvolution::~FilterImporterEvolution() = default;

QString FilterImporterEvolution::defaultFiltersSettingsPath()
{
    return QStringLiteral("%1/.config/evolution/mail/filters.xml").arg(QDir::homePath());
}

void FilterImporterEvolution::parsePartAction(const QDomElement &ruleFilter, MailCommon::MailFilter *filter, parseType type)
{
    for (QDomElement partFilter = ruleFilter.firstChildElement(); !partFilter.isNull(); partFilter = partFilter.nextSiblingElement()) {
        const QString nexttag = partFilter.tagName();
        if (nexttag == QLatin1StringView("part")) {
            if (partFilter.hasAttribute(QStringLiteral("name"))) {
                const QString name = partFilter.attribute(QStringLiteral("name"));
                qCDebug(MAILCOMMON_LOG) << " parsePartAction name attribute :" << name;
                if (type == FilterImporterEvolution::FilterImporterEvolution::parseType::PartType) {
                    QByteArray fieldName;

                    if (name == QLatin1StringView("to")) {
                        fieldName = "to";
                    } else if (name == QLatin1StringView("sender")) {
                        fieldName = "from";
                    } else if (name == QLatin1StringView("cc")) {
                        fieldName = "cc";
                    } else if (name == QLatin1StringView("bcc")) {
                        fieldName = "bcc"; // Verify
                        // TODO
                    } else if (name == QLatin1StringView("senderto")) {
                        // TODO
                    } else if (name == QLatin1StringView("subject")) {
                        fieldName = "subject";
                    } else if (name == QLatin1StringView("header")) {
                        fieldName = "<any header>";
                    } else if (name == QLatin1StringView("body")) {
                        fieldName = "<body>";
                    } else if (name == QLatin1StringView("sexp")) {
                        // TODO
                    } else if (name == QLatin1StringView("sent-date")) {
                        // TODO
                    } else if (name == QLatin1StringView("recv-date")) {
                        fieldName = "<date>";
                    } else if (name == QLatin1StringView("label")) {
                        // TODO
                    } else if (name == QLatin1StringView("score")) {
                        // TODO
                    } else if (name == QLatin1StringView("size")) {
                        fieldName = "<size>";
                    } else if (name == QLatin1StringView("status")) {
                        fieldName = "<status>";
                    } else if (name == QLatin1StringView("follow-up")) {
                        // TODO
                    } else if (name == QLatin1StringView("completed-on")) {
                        // TODO
                    } else if (name == QLatin1StringView("attachments")) {
                        // TODO
                    } else if (name == QLatin1StringView("mlist")) {
                        fieldName = "list-id"; // Verify
                    } else if (name == QLatin1StringView("regex")) {
                        // TODO
                    } else if (name == QLatin1StringView("source")) {
                        // TODO
                    } else if (name == QLatin1StringView("pipe")) {
                        // TODO
                    } else if (name == QLatin1StringView("junk")) {
                        // TODO
                    } else if (name == QLatin1StringView("all")) {
                        filter->pattern()->setOp(SearchPattern::OpAll);
                        break;
                    } else {
                        qCDebug(MAILCOMMON_LOG) << " parttype part : name : not implemented :" << name;
                    }
                    if (fieldName.isEmpty()) {
                        qCDebug(MAILCOMMON_LOG) << " parttype part : name : not implemented :" << name;
                        continue;
                    }
                    QString contents;
                    SearchRule::Function functionName = SearchRule::FuncNone;

                    for (QDomElement valueFilter = partFilter.firstChildElement(); !valueFilter.isNull(); valueFilter = valueFilter.nextSiblingElement()) {
                        const QString valueTag = valueFilter.tagName();

                        if (valueTag == QLatin1StringView("value")) {
                            if (valueFilter.hasAttribute(QStringLiteral("name"))) {
                                const QString valueTagFilterName = valueFilter.attribute(QStringLiteral("name"));
                                if (valueTagFilterName == QLatin1StringView("flag")) {
                                    const QString flag = valueFilter.attribute(QStringLiteral("value"));
                                    qCDebug(MAILCOMMON_LOG) << " flag :" << flag;
                                    if (flag == QLatin1StringView("Seen")) {
                                        contents = QStringLiteral("Read");
                                    } else if (flag == QLatin1StringView("Answered")) {
                                        contents = QStringLiteral("Sent");
                                    } else if (flag == QLatin1StringView("Draft")) {
                                        // FIXME
                                    } else if (flag == QLatin1StringView("Flagged")) { // Important
                                        contents = QStringLiteral("Important");
                                    } else if (flag == QLatin1StringView("Junk")) {
                                        contents = QStringLiteral("Spam");
                                    } else {
                                        qCDebug(MAILCOMMON_LOG) << " unknown status flags " << flag;
                                    }
                                }
                                qCDebug(MAILCOMMON_LOG) << " value filter name :" << valueTagFilterName;
                            }
                            if (valueFilter.hasAttribute(QStringLiteral("type"))) {
                                const QString typeName = valueFilter.attribute(QStringLiteral("type"));
                                if (typeName == QLatin1StringView("option")) {
                                    // Nothing we will look at value
                                } else if (typeName == QLatin1StringView("string")) {
                                    QDomElement string = valueFilter.firstChildElement();
                                    contents = string.text();
                                } else if (typeName == QLatin1StringView("folder")) {
                                    QDomElement folder = valueFilter.firstChildElement();
                                    if (folder.hasAttribute(QStringLiteral("uri"))) {
                                        contents = folder.attribute(QStringLiteral("uri"));
                                        if (!contents.isEmpty()) {
                                            contents.remove(QStringLiteral("folder://"));
                                        }
                                    }
                                } else if (typeName == QLatin1StringView("address")) {
                                    QDomElement address = valueFilter.firstChildElement();
                                    contents = address.text();
                                } else if (typeName == QLatin1StringView("integer")) {
                                    if (valueFilter.hasAttribute(QStringLiteral("integer"))) {
                                        contents = valueFilter.attribute(QStringLiteral("integer"));
                                        int val = contents.toInt();
                                        val = val * 1024; // store in Ko
                                        contents = QString::number(val);
                                    }
                                } else {
                                    qCDebug(MAILCOMMON_LOG) << " type not implemented " << typeName;
                                }
                            }
                            if (valueFilter.hasAttribute(QStringLiteral("value"))) {
                                const QString value = valueFilter.attribute(QStringLiteral("value"));
                                qCDebug(MAILCOMMON_LOG) << " value filter value :" << name;
                                if (value == QLatin1StringView("contains")) {
                                    functionName = SearchRule::FuncContains;
                                } else if (value == QLatin1StringView("not contains")) {
                                    functionName = SearchRule::FuncContainsNot;
                                } else if (value == QLatin1StringView("is not")) {
                                    functionName = SearchRule::FuncNotEqual;
                                } else if (value == QLatin1StringView("is")) {
                                    functionName = SearchRule::FuncEquals;
                                } else if (value == QLatin1StringView("exist")) {
                                    // TODO
                                } else if (value == QLatin1StringView("not exist")) {
                                    // TODO
                                } else if (value == QLatin1StringView("not starts with")) {
                                    functionName = SearchRule::FuncNotStartWith;
                                } else if (value == QLatin1StringView("ends with")) {
                                    functionName = SearchRule::FuncEndWith;
                                } else if (value == QLatin1StringView("not ends with")) {
                                    functionName = SearchRule::FuncNotEndWith;
                                } else if (value == QLatin1StringView("matches soundex")) {
                                    // TODO
                                } else if (value == QLatin1StringView("not match soundex")) {
                                    // TODO
                                } else if (value == QLatin1StringView("before")) {
                                    // TODO
                                } else if (value == QLatin1StringView("after")) {
                                    // TODO
                                } else if (value == QLatin1StringView("greater-than")) {
                                    functionName = SearchRule::FuncIsGreater;
                                } else if (value == QLatin1StringView("less-than")) {
                                    functionName = SearchRule::FuncIsLess;
                                } else if (value == QLatin1StringView("starts with")) {
                                    functionName = SearchRule::FuncStartWith;
                                }
                            }
                        }
                    }
                    SearchRule::Ptr rule = SearchRule::createInstance(fieldName, functionName, contents);
                    filter->pattern()->append(rule);
                } else if (type == FilterImporterEvolution::FilterImporterEvolution::parseType::ActionType) {
                    QString actionName;
                    if (name == QLatin1StringView("stop")) {
                        filter->setStopProcessingHere(true);
                        break;
                    } else if (name == QLatin1StringView("move-to-folder")) {
                        actionName = QStringLiteral("transfer");
                    } else if (name == QLatin1StringView("copy-to-folder")) {
                        actionName = QStringLiteral("copy");
                    } else if (name == QLatin1StringView("delete")) {
                        actionName = QStringLiteral("delete");
                    } else if (name == QLatin1StringView("label")) {
                        // TODO
                    } else if (name == QLatin1StringView("colour")) {
                        // TODO
                    } else if (name == QLatin1StringView("score")) {
                        // TODO
                    } else if (name == QLatin1StringView("adj-score")) {
                        // TODO
                    } else if (name == QLatin1StringView("set-status")) {
                        actionName = QStringLiteral("set status");
                    } else if (name == QLatin1StringView("unset-status")) {
                        actionName = QStringLiteral("unset status");
                    } else if (name == QLatin1StringView("play-sound")) {
                        actionName = QStringLiteral("play sound");
                    } else if (name == QLatin1StringView("shell")) {
                        actionName = QStringLiteral("execute");
                    } else if (name == QLatin1StringView("pipe")) {
                        actionName = QStringLiteral("filter app");
                    } else if (name == QLatin1StringView("forward")) {
                        actionName = QStringLiteral("forward");
                    }
                    if (actionName.isEmpty()) {
                        qCDebug(MAILCOMMON_LOG) << " actiontype part : name : not implemented :" << name;
                    }
                    QString value;
                    for (QDomElement valueFilter = partFilter.firstChildElement(); !valueFilter.isNull(); valueFilter = valueFilter.nextSiblingElement()) {
                        const QString valueTag = valueFilter.tagName();
                        if (valueTag == QLatin1StringView("value")) {
                            if (valueFilter.hasAttribute(QStringLiteral("name"))) {
                                const QString valueFilterName = valueFilter.attribute(QStringLiteral("name"));
                                qCDebug(MAILCOMMON_LOG) << " value filter name :" << valueFilterName;
                            }
                            if (valueFilter.hasAttribute(QStringLiteral("type"))) {
                                const QString filterTypeName = valueFilter.attribute(QStringLiteral("type"));
                                qCDebug(MAILCOMMON_LOG) << " value filter type :" << filterTypeName;
                                if (filterTypeName == QLatin1StringView("option")) {
                                    // Nothing we will look at value
                                } else if (filterTypeName == QLatin1StringView("string")) {
                                    // TODO
                                } else if (filterTypeName == QLatin1StringView("folder")) {
                                    QDomElement folder = valueFilter.firstChildElement();

                                    if (folder.hasAttribute(QStringLiteral("uri"))) {
                                        value = folder.attribute(QStringLiteral("uri"));
                                        if (!value.isEmpty()) {
                                            value.remove(QStringLiteral("folder://"));
                                        }
                                        qCDebug(MAILCOMMON_LOG) << " contents folder :" << value;
                                    }
                                } else if (filterTypeName == QLatin1StringView("address")) {
                                    // TODO
                                }
                            }
                            if (valueFilter.hasAttribute(QStringLiteral("value"))) {
                                const QString valueFilterName = valueFilter.attribute(QStringLiteral("value"));
                                qCDebug(MAILCOMMON_LOG) << " value filter value :" << valueFilterName;
                                if (value == QLatin1StringView("contains")) {
                                    // TODO
                                }
                            }
                        }
                    }
                    createFilterAction(filter, actionName, value);
                }
            }
        }
    }
}

void FilterImporterEvolution::parseFilters(const QDomElement &e)
{
    auto filter = new MailCommon::MailFilter();
    if (e.hasAttribute(QStringLiteral("enabled"))) {
        const QString attr = e.attribute(QStringLiteral("enabled"));
        if (attr == QLatin1StringView("false")) {
            filter->setEnabled(false);
        }
    }

    if (e.hasAttribute(QStringLiteral("grouping"))) {
        const QString attr = e.attribute(QStringLiteral("grouping"));
        if (attr == QLatin1StringView("all")) {
            filter->pattern()->setOp(SearchPattern::OpAnd);
        } else if (attr == QLatin1StringView("any")) {
            filter->pattern()->setOp(SearchPattern::OpOr);
        } else {
            qCDebug(MAILCOMMON_LOG) << " grouping not implemented: " << attr;
        }
    }

    if (e.hasAttribute(QStringLiteral("source"))) {
        const QString attr = e.attribute(QStringLiteral("source"));
        if (attr == QLatin1StringView("incoming")) {
            filter->setApplyOnInbound(true);
        } else if (attr == QLatin1StringView("outgoing")) {
            filter->setApplyOnInbound(false);
            filter->setApplyOnOutbound(true);
        } else {
            qCDebug(MAILCOMMON_LOG) << " source not implemented :" << attr;
        }
    }
    for (QDomElement ruleFilter = e.firstChildElement(); !ruleFilter.isNull(); ruleFilter = ruleFilter.nextSiblingElement()) {
        const QString nexttag = ruleFilter.tagName();
        if (nexttag == QLatin1StringView("title")) {
            filter->pattern()->setName(ruleFilter.text());
            filter->setToolbarName(ruleFilter.text());
        } else if (nexttag == QLatin1StringView("partset")) {
            parsePartAction(ruleFilter, filter, FilterImporterEvolution::parseType::PartType);
        } else if (nexttag == QLatin1StringView("actionset")) {
            parsePartAction(ruleFilter, filter, FilterImporterEvolution::parseType::ActionType);
        } else {
            qCDebug(MAILCOMMON_LOG) << " tag not implemented : " << nexttag;
        }
    }

    appendFilter(filter);
}

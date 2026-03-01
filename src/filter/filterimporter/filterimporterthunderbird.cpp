/*
  SPDX-FileCopyrightText: 2011-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filterimporterthunderbird.h"

#include "filter/mailfilter.h"
#include "mailcommon_debug.h"
#include <MailImporter/FilterIcedove>
#include <MailImporter/FilterSeaMonkey>
#include <MailImporter/FilterThunderbird>
#include <QUrl>

#include <QFile>

using namespace MailCommon;

using namespace Qt::Literals::StringLiterals;
FilterImporterThunderbird::FilterImporterThunderbird(QFile *file, bool interactive)
    : FilterImporterAbstract(interactive)
{
    QTextStream stream(file);
    readStream(stream);
}

FilterImporterThunderbird::FilterImporterThunderbird(QString string, bool interactive)
    : FilterImporterAbstract(interactive)
{
    QTextStream stream(&string);
    readStream(stream);
}

FilterImporterThunderbird::~FilterImporterThunderbird() = default;

void FilterImporterThunderbird::readStream(QTextStream &stream)
{
    MailFilter *filter = nullptr;
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        qCDebug(MAILCOMMON_LOG) << " line :" << line << " filter " << filter;
        filter = parseLine(stream, line, filter);
    }
    // TODO show limit of action/condition
    appendFilter(filter);
}

QString FilterImporterThunderbird::defaultSeaMonkeyFiltersSettingsPath()
{
    return MailImporter::FilterSeaMonkey::defaultSettingsPath();
}

QString FilterImporterThunderbird::defaultIcedoveFiltersSettingsPath()
{
    return MailImporter::FilterIcedove::defaultSettingsPath();
}

QString FilterImporterThunderbird::defaultThunderbirdFiltersSettingsPath()
{
    return MailImporter::FilterThunderbird::defaultSettingsPath();
}

MailCommon::MailFilter *FilterImporterThunderbird::parseLine(QTextStream &stream, QString line, MailCommon::MailFilter *filter)
{
    if (line.startsWith(QLatin1StringView("name="))) {
        appendFilter(filter);
        filter = new MailFilter();
        line = cleanArgument(line, QStringLiteral("name="));
        filter->pattern()->setName(line);
        filter->setToolbarName(line);
    } else if (line.startsWith(QLatin1StringView("action="))) {
        line = cleanArgument(line, QStringLiteral("action="));
        QString value;
        QString actionName = extractActions(line, filter, value);
        if (!stream.atEnd()) {
            line = stream.readLine();
            if (line.startsWith(QLatin1StringView("actionValue="))) {
                value = cleanArgument(line, QStringLiteral("actionValue="));
                // change priority
                if (actionName == QLatin1StringView("Change priority")) {
                    QStringList lstValue;
                    lstValue << QStringLiteral("X-Priority");
                    if (value == QLatin1StringView("Highest")) {
                        value = QStringLiteral("1 (Highest)");
                    } else if (value == QLatin1StringView("High")) {
                        value = QStringLiteral("2 (High)");
                    } else if (value == QLatin1StringView("Normal")) {
                        value = QStringLiteral("3 (Normal)");
                    } else if (value == QLatin1StringView("Low")) {
                        value = QStringLiteral("4 (Low)");
                    } else if (value == QLatin1StringView("Lowest")) {
                        value = QStringLiteral("5 (Lowest)");
                    }
                    lstValue << value;
                    value = lstValue.join(u'\t');
                    actionName = QStringLiteral("add header");
                } else if (actionName == QLatin1StringView("copy") || actionName == QLatin1StringView("transfer")) {
                    QUrl url = QUrl::fromLocalFile(value);
                    if (url.isValid()) {
                        QString path = url.path();
                        if (path.startsWith(u'/')) {
                            path.remove(0, 1); // Remove '/'
                        }
                        value = path;
                    }
                }
                createFilterAction(filter, actionName, value);
            } else {
                createFilterAction(filter, actionName, value);
                filter = parseLine(stream, line, filter);
            }
        } else {
            createFilterAction(filter, actionName, value);
        }
    } else if (line.startsWith(QLatin1StringView("enabled="))) {
        line = cleanArgument(line, QStringLiteral("enabled="));
        if (line == QLatin1StringView("no")) {
            filter->setEnabled(false);
        }
    } else if (line.startsWith(QLatin1StringView("condition="))) {
        line = cleanArgument(line, QStringLiteral("condition="));
        extractConditions(line, filter);
    } else if (line.startsWith(QLatin1StringView("type="))) {
        line = cleanArgument(line, QStringLiteral("type="));
        extractType(line, filter);
    } else if (line.startsWith(QLatin1StringView("version="))) {
        line = cleanArgument(line, QStringLiteral("version="));
        if (line.toInt() != 9) {
            qCDebug(MAILCOMMON_LOG) << " thunderbird filter version different of 9 need to look at if it changed";
        }
    } else if (line.startsWith(QLatin1StringView("logging="))) {
        line = cleanArgument(line, QStringLiteral("logging="));
        if (line == QLatin1StringView("no")) {
            // TODO
        } else if (line == QLatin1StringView("yes")) {
            // TODO
        } else {
            qCDebug(MAILCOMMON_LOG) << " Logging option not implemented " << line;
        }
    } else {
        qCDebug(MAILCOMMON_LOG) << "unknown tag : " << line;
    }
    return filter;
}

void FilterImporterThunderbird::extractConditions(const QString &line, MailCommon::MailFilter *filter)
{
    if (line.startsWith(QLatin1StringView("AND"))) {
        filter->pattern()->setOp(SearchPattern::OpAnd);
        const QStringList conditionsList = line.split(QStringLiteral("AND "));
        const int numberOfCond(conditionsList.count());
        for (int i = 0; i < numberOfCond; ++i) {
            if (!conditionsList.at(i).trimmed().isEmpty()) {
                splitConditions(conditionsList.at(i), filter);
            }
        }
    } else if (line.startsWith(QLatin1StringView("OR"))) {
        filter->pattern()->setOp(SearchPattern::OpOr);
        const QStringList conditionsList = line.split(QStringLiteral("OR "));
        const int numberOfCond(conditionsList.count());
        for (int i = 0; i < numberOfCond; ++i) {
            if (!conditionsList.at(i).trimmed().isEmpty()) {
                splitConditions(conditionsList.at(i), filter);
            }
        }
    } else if (line.startsWith(QLatin1StringView("ALL"))) {
        filter->pattern()->setOp(SearchPattern::OpAll);
    } else {
        qCDebug(MAILCOMMON_LOG) << " missing extract condition" << line;
    }
}

bool FilterImporterThunderbird::splitConditions(const QString &cond, MailCommon::MailFilter *filter)
{
    /*
    *    {nsMsgSearchAttrib::Subject,    "subject"},
    {nsMsgSearchAttrib::Sender,     "from"},
    {nsMsgSearchAttrib::Body,       "body"},
    {nsMsgSearchAttrib::Date,       "date"},
    {nsMsgSearchAttrib::Priority,   "priority"},
    {nsMsgSearchAttrib::MsgStatus,  "status"},
    {nsMsgSearchAttrib::To,         "to"},
    {nsMsgSearchAttrib::CC,         "cc"},
    {nsMsgSearchAttrib::ToOrCC,     "to or cc"},
    {nsMsgSearchAttrib::AllAddresses, "all addresses"},
    {nsMsgSearchAttrib::AgeInDays,  "age in days"},
    {nsMsgSearchAttrib::Label,      "label"},
    {nsMsgSearchAttrib::Keywords,   "tag"},
    {nsMsgSearchAttrib::Size,       "size"},
    // this used to be nsMsgSearchAttrib::SenderInAddressBook
    // we used to have two Sender menuitems
    // for backward compatibility, we can still parse
    // the old style.  see bug #179803
    {nsMsgSearchAttrib::Sender,     "from in ab"},
    {nsMsgSearchAttrib::JunkStatus, "junk status"},
    {nsMsgSearchAttrib::JunkPercent, "junk percent"},
    {nsMsgSearchAttrib::JunkScoreOrigin, "junk score origin"},
    {nsMsgSearchAttrib::HasAttachmentStatus, "has attachment status"},

    */

    QString str = cond.trimmed();
    str.remove(u'(');
    str.remove(str.length() - 1, 1); // remove last )

    const QStringList listOfCond = str.split(u',');
    if (listOfCond.count() < 3) {
        qCDebug(MAILCOMMON_LOG) << "We have a pb in cond:" << cond;
        return false;
    }
    const QString field = listOfCond.at(0);
    const QString function = listOfCond.at(1);
    const QString contents = listOfCond.at(2);

    QByteArray fieldName;
    if (field == QLatin1StringView("subject")) {
        fieldName = "subject";
    } else if (field == QLatin1StringView("from")) {
        fieldName = "from";
    } else if (field == QLatin1StringView("body")) {
        fieldName = "<body>";
    } else if (field == QLatin1StringView("date")) {
        fieldName = "<date>";
    } else if (field == QLatin1StringView("priority")) {
        // TODO
    } else if (field == QLatin1StringView("status")) {
        fieldName = "<status>";
    } else if (field == QLatin1StringView("to")) {
        fieldName = "to";
    } else if (field == QLatin1StringView("cc")) {
        fieldName = "cc";
    } else if (field == QLatin1StringView("to or cc")) {
        fieldName = "<recipients>";
    } else if (field == QLatin1StringView("all addresses")) {
        fieldName = "<recipients>";
    } else if (field == QLatin1StringView("age in days")) {
        fieldName = "<age in days>";
    } else if (field == QLatin1StringView("label")) {
        // TODO
    } else if (field == QLatin1StringView("tag")) {
        fieldName = "<tag>";
    } else if (field == QLatin1StringView("size")) {
        fieldName = "<size>";
    } else if (field == QLatin1StringView("from in ab")) {
        // TODO
    } else if (field == QLatin1StringView("junk status")) {
        // TODO
    } else if (field == QLatin1StringView("junk percent")) {
        // TODO
    } else if (field == QLatin1StringView("junk score origin")) {
        // TODO
    } else if (field == QLatin1StringView("has attachment status")) {
        // TODO
    }

    if (fieldName.isEmpty()) {
        qCDebug(MAILCOMMON_LOG) << " Field not implemented: " << field;
    }
    /*
    {nsMsgSearchOp::Contains, "contains"},
    {nsMsgSearchOp::DoesntContain,"doesn't contain"},
    {nsMsgSearchOp::Is,"is"},
    {nsMsgSearchOp::Isn't,  "isn't"},
    {nsMsgSearchOp::IsEmpty, "is empty"},
    {nsMsgSearchOp::IsntEmpty, "isn't empty"},
    {nsMsgSearchOp::IsBefore, "is before"},
    {nsMsgSearchOp::IsAfter, "is after"},
    {nsMsgSearchOp::IsHigherThan, "is higher than"},
    {nsMsgSearchOp::IsLowerThan, "is lower than"},
    {nsMsgSearchOp::BeginsWith, "begins with"},
    {nsMsgSearchOp::EndsWith, "ends with"},
    {nsMsgSearchOp::IsInAB, "is in ab"},
    {nsMsgSearchOp::IsntInAB, "isn't in ab"},
    {nsMsgSearchOp::IsGreaterThan, "is greater than"},
    {nsMsgSearchOp::IsLessThan, "is less than"},
    {nsMsgSearchOp::Matches, "matches"},
    {nsMsgSearchOp::DoesntMatch, "doesn't match"}
    */
    SearchRule::Function functionName = SearchRule::FuncNone;

    if (function == QLatin1StringView("contains")) {
        functionName = SearchRule::FuncContains;
    } else if (function == QLatin1StringView("doesn't contain")) {
        functionName = SearchRule::FuncContainsNot;
    } else if (function == QLatin1StringView("is")) {
        functionName = SearchRule::FuncEquals;
    } else if (function == QLatin1StringView("isn't")) {
        functionName = SearchRule::FuncNotEqual;
    } else if (function == QLatin1StringView("is empty")) {
        // TODO
    } else if (function == QLatin1StringView("isn't empty")) {
        // TODO
    } else if (function == QLatin1StringView("is before")) {
        functionName = SearchRule::FuncIsLess;
    } else if (function == QLatin1StringView("is after")) {
        functionName = SearchRule::FuncIsGreater;
    } else if (function == QLatin1StringView("is higher than")) {
        functionName = SearchRule::FuncIsGreater;
    } else if (function == QLatin1StringView("is lower than")) {
        functionName = SearchRule::FuncIsLess;
    } else if (function == QLatin1StringView("begins with")) {
        functionName = SearchRule::FuncStartWith;
    } else if (function == QLatin1StringView("ends with")) {
        functionName = SearchRule::FuncEndWith;
    } else if (function == QLatin1StringView("is in ab")) {
        functionName = SearchRule::FuncIsInAddressbook;
    } else if (function == QLatin1StringView("isn't in ab")) {
        functionName = SearchRule::FuncIsNotInAddressbook;
    } else if (function == QLatin1StringView("is greater than")) {
        functionName = SearchRule::FuncIsGreater;
    } else if (function == QLatin1StringView("is less than")) {
        functionName = SearchRule::FuncIsLess;
    } else if (function == QLatin1StringView("matches")) {
        functionName = SearchRule::FuncEquals;
    } else if (function == QLatin1StringView("doesn't match")) {
        functionName = SearchRule::FuncNotEqual;
    }

    if (functionName == SearchRule::FuncNone) {
        qCDebug(MAILCOMMON_LOG) << " functionName not implemented: " << function;
    }
    QString contentsName;
    if (fieldName == "<status>") {
        if (contents == QLatin1StringView("read")) {
            contentsName = QStringLiteral("Read");
        } else if (contents == QLatin1StringView("unread")) {
            contentsName = QStringLiteral("Unread");
        } else if (contents == QLatin1StringView("new")) {
            contentsName = QStringLiteral("New");
        } else if (contents == QLatin1StringView("forwarded")) {
            contentsName = QStringLiteral("Forwarded");
        } else {
            qCDebug(MAILCOMMON_LOG) << " contents for status not implemented " << contents;
        }
    } else if (fieldName == "<size>") {
        int value = contents.toInt();
        value = value * 1024; // Ko
        contentsName = QString::number(value);
    } else if (fieldName == "<date>") {
        QLocale locale(QLocale::C);
        const QDate date = locale.toDate(contents, QStringLiteral("dd-MMM-yyyy"));
        contentsName = date.toString(Qt::ISODate);
    } else {
        contentsName = contents;
    }

    SearchRule::Ptr rule = SearchRule::createInstance(fieldName, functionName, contentsName);
    filter->pattern()->append(rule);
    // qCDebug(MAILCOMMON_LOG) << " field :" << field << " function :" << function
    //         << " contents :" << contents << " cond :" << cond;
    return true;
}

QString FilterImporterThunderbird::extractActions(const QString &line, MailCommon::MailFilter *filter, QString &value)
{
    /*
    { nsMsgFilterAction::MoveToFolder,            "Move to folder"},
    { nsMsgFilterAction::CopyToFolder,            "Copy to folder"},
    { nsMsgFilterAction::ChangePriority,          "Change priority"},
    { nsMsgFilterAction::Delete,                  "Delete"},
    { nsMsgFilterAction::MarkRead,                "Mark read"},
    { nsMsgFilterAction::KillThread,              "Ignore thread"},
    { nsMsgFilterAction::KillSubthread,           "Ignore subthread"},
    { nsMsgFilterAction::WatchThread,             "Watch thread"},
    { nsMsgFilterAction::MarkFlagged,             "Mark flagged"},
    { nsMsgFilterAction::Label,                   "Label"},
    { nsMsgFilterAction::Reply,                   "Reply"},
    { nsMsgFilterAction::Forward,                 "Forward"},
    { nsMsgFilterAction::StopExecution,           "Stop execution"},
    { nsMsgFilterAction::DeleteFromPop3Server,    "Delete from Pop3 server"},
    { nsMsgFilterAction::LeaveOnPop3Server,       "Leave on Pop3 server"},
    { nsMsgFilterAction::JunkScore,               "JunkScore"},
    { nsMsgFilterAction::FetchBodyFromPop3Server, "Fetch body from Pop3Server"},
    { nsMsgFilterAction::AddTag,                  "AddTag"},
    { nsMsgFilterAction::Custom,                  "Custom"},
     */

    QString actionName;
    if (line == QLatin1StringView("Move to folder")) {
        actionName = QStringLiteral("transfer");
    } else if (line == QLatin1StringView("Forward")) {
        actionName = QStringLiteral("forward");
    } else if (line == QLatin1StringView("Mark read")) {
        actionName = QStringLiteral("set status");
        value = QStringLiteral("R");
    } else if (line == QLatin1StringView("Mark unread")) {
        actionName = QStringLiteral("set status");
        value = QStringLiteral("U"); // TODO verify
    } else if (line == QLatin1StringView("Copy to folder")) {
        actionName = QStringLiteral("copy");
    } else if (line == QLatin1StringView("AddTag")) {
        actionName = QStringLiteral("add tag");
    } else if (line == QLatin1StringView("Delete")) {
        actionName = QStringLiteral("delete");
    } else if (line == QLatin1StringView("Change priority")) {
        actionName = QStringLiteral("Change priority"); // Doesn't exist in kmail but we help us to importing
    } else if (line == QLatin1StringView("Ignore thread")) {
    } else if (line == QLatin1StringView("Ignore subthread")) {
    } else if (line == QLatin1StringView("Watch thread")) {
    } else if (line == QLatin1StringView("Mark flagged")) {
    } else if (line == QLatin1StringView("Label")) {
    } else if (line == QLatin1StringView("Reply")) {
        actionName = QStringLiteral("set Reply-To");
    } else if (line == QLatin1StringView("Stop execution")) {
        filter->setStopProcessingHere(true);
        return {};
    } else if (line == QLatin1StringView("Delete from Pop3 server")) {
    } else if (line == QLatin1StringView("JunkScore")) {
    } else if (line == QLatin1StringView("Fetch body from Pop3Server")) {
    } else if (line == QLatin1StringView("Custom")) {
    }
    if (actionName.isEmpty()) {
        qCDebug(MAILCOMMON_LOG) << QStringLiteral(" missing convert method: %1").arg(line);
    }
    return actionName;
}

void FilterImporterThunderbird::extractType(const QString &line, MailCommon::MailFilter *filter)
{
    const int value = line.toInt();
    if (value == 1) {
        filter->setApplyOnInbound(true);
        filter->setApplyOnExplicit(false);
        // Checking mail
    } else if (value == 16) {
        filter->setApplyOnInbound(false);
        filter->setApplyOnExplicit(true);
        // Manual mail
    } else if (value == 17) {
        filter->setApplyOnInbound(true);
        filter->setApplyOnExplicit(true);
        // Checking mail or manual
    } else if (value == 32) {
        filter->setApplyOnExplicit(false);
        filter->setApplyOnOutbound(true);
        filter->setApplyOnInbound(false);
        // checking mail after classification
    } else if (value == 48) {
        filter->setApplyOnExplicit(true);
        filter->setApplyOnOutbound(true);
        filter->setApplyOnInbound(false);
        // checking mail after classification or manual check
    } else {
        qCDebug(MAILCOMMON_LOG) << " type value is not valid :" << value;
    }
}

QString FilterImporterThunderbird::cleanArgument(const QString &line, const QString &removeStr)
{
    QString str = line;
    str.remove(removeStr);
    str.remove(QStringLiteral("\""));
    str.remove(str.length(), 1); // remove last "
    return str;
}

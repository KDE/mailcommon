/*

  SPDX-FileCopyrightText: Marc Mutz <mutz@kde.org>
  SPDX-FileCopyrightText: 2012 Andras Mantia <amantia@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "searchpattern.h"
#include "filter/filterlog.h"
using MailCommon::FilterLog;
#include "mailcommon_debug.h"
#include <Akonadi/ContactSearchJob>

#include <KMime/Message>

#include <KConfigGroup>

#include <QDataStream>
#include <QIODevice>

#include <algorithm>

using namespace MailCommon;

//==================================================
//
// class SearchPattern
//
//==================================================

SearchPattern::SearchPattern()

{
    init();
}

SearchPattern::SearchPattern(const KConfigGroup &config)

{
    readConfig(config);
}

SearchPattern::~SearchPattern() = default;

bool SearchPattern::matches(const Akonadi::Item &item, bool ignoreBody) const
{
    if (isEmpty()) {
        return true;
    }
    if (!item.hasPayload<KMime::Message::Ptr>()) {
        return false;
    }

    QList<SearchRule::Ptr>::const_iterator it;
    QList<SearchRule::Ptr>::const_iterator end(constEnd());
    switch (mOperator) {
    case OpAnd: // all rules must match
        for (it = constBegin(); it != end; ++it) {
            if (!((*it)->requiredPart() == SearchRule::CompleteMessage && ignoreBody)) {
                if (!(*it)->matches(item)) {
                    return false;
                }
            }
        }
        return true;

    case OpOr: // at least one rule must match
        for (it = constBegin(); it != end; ++it) {
            if (!((*it)->requiredPart() == MailCommon::SearchRule::CompleteMessage && ignoreBody)) {
                if ((*it)->matches(item)) {
                    return true;
                }
            }
        }
        return false;

    case OpAll:
        return true;

    default:
        return false;
    }
}

SearchRule::RequiredPart SearchPattern::requiredPart() const
{
    SearchRule::RequiredPart reqPart = SearchRule::Envelope;

    if (!isEmpty()) {
        reqPart = (*std::max_element(constBegin(), constEnd(), [](const auto &lhs, const auto &rhs) {
                      return lhs->requiredPart() < rhs->requiredPart();
                  }))->requiredPart();
    }
    return reqPart;
}

QString SearchPattern::purify(bool removeAction)
{
    QString informationAboutNotValidPattern;
    QList<SearchRule::Ptr>::iterator it = end();
    while (it != begin()) {
        --it;
        if ((*it)->isEmpty()) {
            if (removeAction) {
                qCDebug(MAILCOMMON_LOG) << "Removing" << (*it)->asString();
                if (!informationAboutNotValidPattern.isEmpty()) {
                    informationAboutNotValidPattern += QLatin1Char('\n');
                }
                informationAboutNotValidPattern += (*it)->informationAboutNotValidRules();

                erase(it);
                it = end();
            }
        }
    }

    return informationAboutNotValidPattern;
}

void SearchPattern::readConfig(const KConfigGroup &config)
{
    init();

    mName = config.readEntry("name");
    if (!config.hasKey("rules")) {
        qCDebug(MAILCOMMON_LOG) << "Found legacy config! Converting.";
        importLegacyConfig(config);
        return;
    }

    const QString op = config.readEntry("operator");
    if (op == QLatin1StringView("or")) {
        mOperator = OpOr;
    } else if (op == QLatin1StringView("and")) {
        mOperator = OpAnd;
    } else if (op == QLatin1StringView("all")) {
        mOperator = OpAll;
    }

    const int nRules = config.readEntry("rules", 0);

    for (int i = 0; i < nRules; ++i) {
        SearchRule::Ptr r = SearchRule::createInstanceFromConfig(config, i);
        if (!r->isEmpty()) {
            append(r);
        }
    }
}

void SearchPattern::importLegacyConfig(const KConfigGroup &config)
{
    SearchRule::Ptr rule =
        SearchRule::createInstance(config.readEntry("fieldA").toLatin1(), config.readEntry("funcA").toLatin1().constData(), config.readEntry("contentsA"));

    if (rule->isEmpty()) {
        // if the first rule is invalid,
        // we really can't do much heuristics...
        return;
    }
    append(rule);

    const QString sOperator = config.readEntry("operator");
    if (sOperator == QLatin1StringView("ignore")) {
        return;
    }

    rule = SearchRule::createInstance(config.readEntry("fieldB").toLatin1(), config.readEntry("funcB").toLatin1().constData(), config.readEntry("contentsB"));

    if (rule->isEmpty()) {
        return;
    }
    append(rule);

    if (sOperator == QLatin1StringView("or")) {
        mOperator = OpOr;
        return;
    }
    // This is the interesting case...
    if (sOperator == QLatin1StringView("unless")) { // meaning "and not", ie we need to...
        // ...invert the function (e.g. "equals" <-> "doesn't equal")
        // We simply toggle the last bit (xor with 0x1)... This assumes that
        // SearchRule::Function's come in adjacent pairs of pros and cons
        SearchRule::Function func = last()->function();
        auto intFunc = (unsigned int)func;
        func = SearchRule::Function(intFunc ^ 0x1);

        last()->setFunction(func);
    }

    // treat any other case as "and" (our default).
}

void SearchPattern::writeConfig(KConfigGroup &config) const
{
    config.writeEntry("name", mName);
    switch (mOperator) {
    case OpOr:
        config.writeEntry("operator", "or");
        break;
    case OpAnd:
        config.writeEntry("operator", "and");
        break;
    case OpAll:
        config.writeEntry("operator", "all");
        break;
    }

    int i = 0;
    QList<SearchRule::Ptr>::const_iterator it;
    QList<SearchRule::Ptr>::const_iterator endIt(constEnd());

    if (count() >= filterRulesMaximumSize()) {
        qCDebug(MAILCOMMON_LOG) << "Number of patterns > to filter max rules";
    }
    for (it = constBegin(); it != endIt && i < filterRulesMaximumSize(); ++i, ++it) {
        // we could do this ourselves, but we want the rules to be extensible,
        // so we give the rule it's number and let it do the rest.
        (*it)->writeConfig(config, i);
    }

    // save the total number of rules.
    config.writeEntry("rules", i);
}

int SearchPattern::filterRulesMaximumSize()
{
    return 8;
}

void SearchPattern::init()
{
    clear();
    mOperator = OpAnd;
    mName = QLatin1Char('<') + i18nc("name used for a virgin filter", "unknown") + QLatin1Char('>');
}

QString SearchPattern::asString() const
{
    QString result;
    switch (mOperator) {
    case OpOr:
        result = i18n("(match any of the following)");
        break;
    case OpAnd:
        result = i18n("(match all of the following)");
        break;
    case OpAll:
        result = i18n("(match all messages)");
        break;
    }

    QList<SearchRule::Ptr>::const_iterator it;
    QList<SearchRule::Ptr>::const_iterator endIt = constEnd();
    for (it = constBegin(); it != endIt; ++it) {
        result += QLatin1StringView("\n\t") + FilterLog::recode((*it)->asString());
    }

    return result;
}

SearchPattern::SparqlQueryError SearchPattern::asAkonadiQuery(Akonadi::SearchQuery &query) const
{
    query = Akonadi::SearchQuery();

    Akonadi::SearchTerm term(Akonadi::SearchTerm::RelAnd);
    if (op() == SearchPattern::OpOr) {
        term = Akonadi::SearchTerm(Akonadi::SearchTerm::RelOr);
    }

    const_iterator end(constEnd());
    bool emptyIsNotAnError = false;
    bool resultAddQuery = false;
    for (const_iterator it = constBegin(); it != end; ++it) {
        (*it)->addQueryTerms(term, emptyIsNotAnError);
        resultAddQuery &= emptyIsNotAnError;
    }

    if (term.subTerms().isEmpty()) {
        if (resultAddQuery) {
            qCDebug(MAILCOMMON_LOG) << " innergroup is Empty. Need to report bug";
            return MissingCheck;
        } else {
            return EmptyResult;
        }
    }
    query.setTerm(term);

    return NoError;
}

const SearchPattern &SearchPattern::operator=(const SearchPattern &other)
{
    if (this == &other) {
        return *this;
    }

    setOp(other.op());
    setName(other.name());

    clear(); // ###
    QList<SearchRule::Ptr>::const_iterator it;
    QList<SearchRule::Ptr>::const_iterator end(other.constEnd());
    for (it = other.constBegin(); it != end; ++it) {
        append(SearchRule::createInstance(**it)); // deep copy
    }

    return *this;
}

QByteArray SearchPattern::serialize() const
{
    QByteArray out;
    QDataStream stream(&out, QIODevice::WriteOnly);
    *this >> stream;
    return out;
}

void SearchPattern::deserialize(const QByteArray &str)
{
    QDataStream stream(str);
    *this << stream;
}

QDataStream &SearchPattern::operator>>(QDataStream &s) const
{
    switch (op()) {
    case SearchPattern::OpAnd:
        s << QStringLiteral("and");
        break;
    case SearchPattern::OpOr:
        s << QStringLiteral("or");
        break;
    case SearchPattern::OpAll:
        s << QStringLiteral("all");
        break;
    }

    for (const SearchRule::Ptr &rule : std::as_const(*this)) {
        *rule >> s;
    }
    return s;
}

QDataStream &SearchPattern::operator<<(QDataStream &s)
{
    QString op;
    s >> op;
    if (op == QLatin1StringView("and")) {
        setOp(OpAnd);
    } else if (op == QLatin1StringView("or")) {
        setOp(OpOr);
    } else if (op == QLatin1StringView("all")) {
        setOp(OpAll);
    }

    while (!s.atEnd()) {
        SearchRule::Ptr rule = SearchRule::createInstance(s);
        append(rule);
    }
    return s;
}

void SearchPattern::generateSieveScript(QStringList &requiresModules, QString &code)
{
    code += QLatin1StringView("\n#") + mName + QLatin1Char('\n');
    switch (mOperator) {
    case OpOr:
        code += QLatin1StringView("if anyof (");
        break;
    case OpAnd:
        code += QLatin1StringView("if allof (");
        break;
    case OpAll:
        code += QLatin1StringView("if (true) {");
        return;
    }

    QList<SearchRule::Ptr>::const_iterator it;
    QList<SearchRule::Ptr>::const_iterator endIt(constEnd());
    int i = 0;
    for (it = constBegin(); it != endIt && i < filterRulesMaximumSize(); ++i, ++it) {
        if (i != 0) {
            code += QLatin1StringView("\n, ");
        }
        (*it)->generateSieveScript(requiresModules, code);
    }
}

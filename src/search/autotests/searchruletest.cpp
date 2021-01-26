/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "searchruletest.h"
#include "../search/searchrule/searchrule.h"

#include <QTest>

class TestSearchRule : public MailCommon::SearchRule
{
public:
    TestSearchRule(const QByteArray &field = QByteArray(), Function function = FuncContains, const QString &contents = QString())
        : MailCommon::SearchRule(field, function, contents)
    {
    }

    bool matches(const Akonadi::Item &item) const override
    {
        Q_UNUSED(item)
        return false;
    }

    bool isEmpty() const override
    {
        return false;
    }

    MailCommon::SearchRule::RequiredPart requiredPart() const override
    {
        return MailCommon::SearchRule::CompleteMessage;
    }
};

SearchRuleTest::SearchRuleTest(QObject *parent)
    : QObject(parent)
{
}

void SearchRuleTest::shouldHaveDefaultValue()
{
    TestSearchRule searchrule;
    QCOMPARE(searchrule.field(), QByteArray());
    QCOMPARE(searchrule.function(), MailCommon::SearchRule::FuncContains);
    QVERIFY(searchrule.contents().isEmpty());
}

void SearchRuleTest::shouldAssignValue()
{
    TestSearchRule searchrule;
    // TODO
}

QTEST_MAIN(SearchRuleTest)

/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "mailcommon_private_export.h"
#include "searchpattern.h"

namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT SearchRuleInvitation : public SearchRule
{
public:
    explicit SearchRuleInvitation(const QByteArray &field = {}, Function func = FuncEquals, const QString &contents = {});
    ~SearchRuleInvitation() override;

    [[nodiscard]] bool isEmpty() const override;
    [[nodiscard]] bool matches(const Akonadi::Item &item) const override;
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;
    void addQueryTerms(Akonadi::SearchTerm &groupTerm, bool &emptyIsNotAnError) const override;
};
}

/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "searchpattern.h"

namespace MailCommon
{
class SearchRuleAttachment : public SearchRule
{
public:
    explicit SearchRuleAttachment(const QByteArray &field = {}, Function func = FuncEquals, const QString &contents = {});
    ~SearchRuleAttachment() override;

    [[nodiscard]] bool isEmpty() const override;
    [[nodiscard]] bool matches(const Akonadi::Item &item) const override;
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;
    void addQueryTerms(Akonadi::SearchTerm &groupTerm, bool &emptyIsNotAnError) const override;
};
}

/*
  SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "mailcommon_private_export.h"
#include "searchpattern.h"

namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT SearchRuleEncryption : public SearchRule
{
public:
    explicit SearchRuleEncryption(const QByteArray &field = {}, Function func = FuncEquals, const QString &contents = {});
    ~SearchRuleEncryption() override;

    [[nodiscard]] bool isEmpty() const override;
    [[nodiscard]] bool matches(const Akonadi::Item &item) const override;
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;
};
}

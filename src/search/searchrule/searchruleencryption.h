/*
  SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_SEARCH_SEARCHRULEENCRYPTION_H_
#define MAILCOMMON_SEARCH_SEARCHRULEENCRYPTION_H_

#include "searchpattern.h"

namespace MailCommon
{
class SearchRuleEncryption : public SearchRule
{
public:
    explicit SearchRuleEncryption(const QByteArray &field = {}, Function func = FuncEquals, const QString &contents = {});
    ~SearchRuleEncryption() override;

    bool isEmpty() const override;
    bool matches(const Akonadi::Item &item) const override;
    SearchRule::RequiredPart requiredPart() const override;
};
}

#endif

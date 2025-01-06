/*

  SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "filteractionstatus.h"
#include "mailcommon_private_export.h"
namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT FilterActionUnsetStatus : public FilterActionStatus
{
    Q_OBJECT
public:
    explicit FilterActionUnsetStatus(QObject *parent = nullptr);
    [[nodiscard]] ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;

    static FilterAction *newAction();
    [[nodiscard]] QString sieveCode() const override;
    [[nodiscard]] QStringList sieveRequires() const override;
    [[nodiscard]] bool isEmpty() const override;
    [[nodiscard]] QString informationAboutNotValidAction() const override;

private:
    [[nodiscard]] MAILCOMMON_NO_EXPORT bool checkIsValid(int &index) const;
};
}

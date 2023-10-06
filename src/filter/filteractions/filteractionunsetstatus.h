/*

  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "filteractionstatus.h"
namespace MailCommon
{
class FilterActionUnsetStatus : public FilterActionStatus
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
    bool checkIsValid(int &index) const;
};
}

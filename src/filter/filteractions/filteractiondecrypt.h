/*
 * SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteractionwithcrypto.h"
#include "mailcommon_private_export.h"
namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT FilterActionDecrypt : public FilterActionWithCrypto
{
    Q_OBJECT
public:
    explicit FilterActionDecrypt(QObject *parent = nullptr);
    ~FilterActionDecrypt() override;

    static FilterAction *newAction();

    [[nodiscard]] QString displayString() const override;

    [[nodiscard]] QString argsAsString() const override;
    void argsFromString(const QString &argsStr) override;

    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;
    [[nodiscard]] FilterAction::ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;

    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;
};
} // namespace MailCommon

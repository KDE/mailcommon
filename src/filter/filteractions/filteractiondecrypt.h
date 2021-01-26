/*
 * SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTION_DECRYPT_H_
#define MAILCOMMON_FILTERACTION_DECRYPT_H_

#include "filteractionwithcrypto.h"

namespace MailCommon
{
class FilterActionDecrypt : public FilterActionWithCrypto
{
    Q_OBJECT
public:
    explicit FilterActionDecrypt(QObject *parent = nullptr);
    ~FilterActionDecrypt() override;

    static FilterAction *newAction();

    Q_REQUIRED_RESULT QString displayString() const override;

    Q_REQUIRED_RESULT QString argsAsString() const override;
    void argsFromString(const QString &argsStr) override;

    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;
    Q_REQUIRED_RESULT FilterAction::ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;

    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
};
} // namespace MailCommon

#endif

/*
 * Copyright (c) 2017  Daniel Vrátil <dvratil@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef MAILCOMMON_FILTERACTION_DECRYPT_H_
#define MAILCOMMON_FILTERACTION_DECRYPT_H_

#include "filteractionwithcrypto.h"

namespace MailCommon {
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

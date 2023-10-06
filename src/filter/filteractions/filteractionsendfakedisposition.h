/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteractionwithstringlist.h"

namespace MailCommon
{
//=============================================================================
// FilterActionSendFakeDisposition - send fake MDN
// Sends a fake MDN or forces an ignore.
//=============================================================================
class FilterActionSendFakeDisposition : public FilterActionWithStringList
{
    Q_OBJECT
public:
    explicit FilterActionSendFakeDisposition(QObject *parent = nullptr);
    [[nodiscard]] ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;

    static FilterAction *newAction();

    [[nodiscard]] bool isEmpty() const override;

    void argsFromString(const QString &argsStr) override;
    [[nodiscard]] QString argsAsString() const override;
    [[nodiscard]] QString displayString() const override;
    [[nodiscard]] QString informationAboutNotValidAction() const override;
};
}

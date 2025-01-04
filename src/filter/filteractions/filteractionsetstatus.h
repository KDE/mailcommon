/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteractionstatus.h"
#include "mailcommon_private_export.h"
namespace MailCommon
{
//=============================================================================
// FilterActionSetStatus - set status to
// Set the status of messages
//=============================================================================
class MAILCOMMON_TESTS_EXPORT FilterActionSetStatus : public FilterActionStatus
{
    Q_OBJECT
public:
    explicit FilterActionSetStatus(QObject *parent = nullptr);
    [[nodiscard]] ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;

    static FilterAction *newAction();
    [[nodiscard]] QString sieveCode() const override;
    [[nodiscard]] QStringList sieveRequires() const override;
    [[nodiscard]] bool isEmpty() const override;
    [[nodiscard]] QString informationAboutNotValidAction() const override;
};
}

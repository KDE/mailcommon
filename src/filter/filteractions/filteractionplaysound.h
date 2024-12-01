/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteractionwithtest.h"
#include "mailcommon_private_export.h"

class QMediaPlayer;

namespace MailCommon
{
//=============================================================================
// FilterActionPlaySound - execute command
// Execute a sound
//=============================================================================
class MAILCOMMON_TESTS_EXPORT FilterActionPlaySound : public FilterActionWithTest
{
    Q_OBJECT
public:
    FilterActionPlaySound();
    ~FilterActionPlaySound() override;
    [[nodiscard]] ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
    [[nodiscard]] bool argsFromStringInteractive(const QString &argsStr, const QString &filterName) override;

    [[nodiscard]] bool isEmpty() const override;
    [[nodiscard]] QString informationAboutNotValidAction() const override;

private:
    mutable QMediaPlayer *mPlayer = nullptr;
};
}

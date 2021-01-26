/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONPLAYSOUND_H
#define MAILCOMMON_FILTERACTIONPLAYSOUND_H

#include "filteractionwithtest.h"

namespace Phonon
{
class MediaObject;
}

namespace MailCommon
{
//=============================================================================
// FilterActionPlaySound - execute command
// Execute a sound
//=============================================================================
class FilterActionPlaySound : public FilterActionWithTest
{
    Q_OBJECT
public:
    FilterActionPlaySound();
    ~FilterActionPlaySound() override;
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
    Q_REQUIRED_RESULT bool argsFromStringInteractive(const QString &argsStr, const QString &filterName) override;

    Q_REQUIRED_RESULT bool isEmpty() const override;
    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;

private:
    mutable Phonon::MediaObject *mPlayer = nullptr;
};
}

#endif

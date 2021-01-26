/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONSETSTATUS_H
#define MAILCOMMON_FILTERACTIONSETSTATUS_H

#include "filteractionstatus.h"
namespace MailCommon
{
//=============================================================================
// FilterActionSetStatus - set status to
// Set the status of messages
//=============================================================================
class FilterActionSetStatus : public FilterActionStatus
{
    Q_OBJECT
public:
    explicit FilterActionSetStatus(QObject *parent = nullptr);
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;

    static FilterAction *newAction();
    Q_REQUIRED_RESULT QString sieveCode() const override;
    Q_REQUIRED_RESULT QStringList sieveRequires() const override;
    Q_REQUIRED_RESULT bool isEmpty() const override;
    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;
};
}

#endif

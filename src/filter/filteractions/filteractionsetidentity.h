/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONSETIDENTITY_H
#define MAILCOMMON_FILTERACTIONSETIDENTITY_H

#include "filteractionwithuoid.h"

namespace MailCommon
{
//=============================================================================
// FilterActionSetIdentity - set identity to
// Specify Identity to be used when replying to a message
//=============================================================================
class FilterActionSetIdentity : public FilterActionWithUOID
{
    Q_OBJECT
public:
    explicit FilterActionSetIdentity(QObject *parent = nullptr);
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;
    Q_REQUIRED_RESULT bool argsFromStringInteractive(const QString &argsStr, const QString &filterName) override;
    static FilterAction *newAction();

    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    void applyParamWidgetValue(QWidget *parent) override;
    void setParamWidgetValue(QWidget *parent) const override;
    void clearParamWidget(QWidget *param) const override;
};
}

#endif

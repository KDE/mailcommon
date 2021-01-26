/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONDELETE_H
#define MAILCOMMON_FILTERACTIONDELETE_H

#include "filteractionwithnone.h"

namespace MailCommon
{
//=============================================================================
// FilterActionDelete - Delete action
//=============================================================================
class FilterActionDelete : public FilterActionWithNone
{
    Q_OBJECT
public:
    explicit FilterActionDelete(QObject *parent = nullptr);
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    Q_REQUIRED_RESULT QString sieveCode() const override;
};
}

#endif

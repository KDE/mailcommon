/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONMOVE_H
#define MAILCOMMON_FILTERACTIONMOVE_H

#include "filteractionwithfolder.h"

namespace MailCommon
{
//=============================================================================
// FilterActionMove - move into folder
// File message into another mail folder
//=============================================================================
class FilterActionMove : public FilterActionWithFolder
{
    Q_OBJECT
public:
    explicit FilterActionMove(QObject *parent = nullptr);
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
    Q_REQUIRED_RESULT QString sieveCode() const override;
    Q_REQUIRED_RESULT QStringList sieveRequires() const override;

    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;
};
}

#endif

/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONCOPY_H
#define MAILCOMMON_FILTERACTIONCOPY_H

#include "filteractionwithfolder.h"

class KJob;

namespace MailCommon
{
//=============================================================================
// FilterActionCopy - copy into folder
// Copy message into another mail folder
//=============================================================================
class FilterActionCopy : public FilterActionWithFolder
{
    Q_OBJECT
public:
    explicit FilterActionCopy(QObject *parent = nullptr);
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
    Q_REQUIRED_RESULT QString sieveCode() const override;
    Q_REQUIRED_RESULT QStringList sieveRequires() const override;
    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;

protected Q_SLOTS:
    void jobFinished(KJob *job);
};
}

#endif

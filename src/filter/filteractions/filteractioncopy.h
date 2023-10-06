/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

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
    [[nodiscard]] ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
    [[nodiscard]] QString sieveCode() const override;
    [[nodiscard]] QStringList sieveRequires() const override;
    [[nodiscard]] QString informationAboutNotValidAction() const override;

protected Q_SLOTS:
    void jobFinished(KJob *job);
};
}

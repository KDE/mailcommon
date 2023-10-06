/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

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
    [[nodiscard]] ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;
    static FilterAction *newAction();
    [[nodiscard]] QString sieveCode() const override;
    [[nodiscard]] QStringList sieveRequires() const override;

    [[nodiscard]] QString informationAboutNotValidAction() const override;
};
}

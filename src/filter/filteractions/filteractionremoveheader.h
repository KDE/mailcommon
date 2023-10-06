/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteractionwithstringlist.h"

namespace MailCommon
{
//=============================================================================
// FilterActionRemoveHeader - remove header
// Remove all instances of the given header field.
//=============================================================================
class FilterActionRemoveHeader : public FilterActionWithStringList
{
    Q_OBJECT
public:
    explicit FilterActionRemoveHeader(QObject *parent = nullptr);
    [[nodiscard]] ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;
    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QWidget *paramWidget) const override;
    [[nodiscard]] QStringList sieveRequires() const override;
    [[nodiscard]] QString sieveCode() const override;

    static FilterAction *newAction();
    [[nodiscard]] QString informationAboutNotValidAction() const override;
};
}

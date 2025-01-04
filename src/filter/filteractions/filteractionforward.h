/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteractionwithaddress.h"
#include "mailcommon_private_export.h"
namespace MailCommon
{
//=============================================================================
// FilterActionForward - forward to
// Forward message to another user, with a defined template
//=============================================================================
class MAILCOMMON_TESTS_EXPORT FilterActionForward : public FilterActionWithAddress
{
    Q_OBJECT
public:
    explicit FilterActionForward(QObject *parent = nullptr);
    static FilterAction *newAction();
    [[nodiscard]] ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;
    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;
    void applyParamWidgetValue(QWidget *paramWidget) override;
    void setParamWidgetValue(QWidget *paramWidget) const override;
    void clearParamWidget(QWidget *paramWidget) const override;
    void argsFromString(const QString &argsStr) override;
    [[nodiscard]] QString argsAsString() const override;
    [[nodiscard]] QString displayString() const override;
    [[nodiscard]] bool argsFromStringInteractive(const QString &argsStr, const QString &filterName) override;

    [[nodiscard]] QString informationAboutNotValidAction() const override;

private:
    mutable QString mTemplate;
};
}

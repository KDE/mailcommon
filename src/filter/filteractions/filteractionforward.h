/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONFORWARD_H
#define MAILCOMMON_FILTERACTIONFORWARD_H

#include "filteractionwithaddress.h"

namespace MailCommon
{
//=============================================================================
// FilterActionForward - forward to
// Forward message to another user, with a defined template
//=============================================================================
class FilterActionForward : public FilterActionWithAddress
{
    Q_OBJECT
public:
    explicit FilterActionForward(QObject *parent = nullptr);
    static FilterAction *newAction();
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;
    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    void applyParamWidgetValue(QWidget *paramWidget) override;
    void setParamWidgetValue(QWidget *paramWidget) const override;
    void clearParamWidget(QWidget *paramWidget) const override;
    void argsFromString(const QString &argsStr) override;
    Q_REQUIRED_RESULT QString argsAsString() const override;
    Q_REQUIRED_RESULT QString displayString() const override;
    Q_REQUIRED_RESULT bool argsFromStringInteractive(const QString &argsStr, const QString &filterName) override;

    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;

private:
    mutable QString mTemplate;
};
}

#endif

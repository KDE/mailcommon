/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteractionwithstringlist.h"
#include "mailcommon_private_export.h"
namespace MailCommon
{
//=============================================================================
// FilterActionAddHeader - add header
// Add a header with the given value.
//=============================================================================
class MAILCOMMON_TESTS_EXPORT FilterActionAddHeader : public FilterActionWithStringList
{
    Q_OBJECT
public:
    explicit FilterActionAddHeader(QObject *parent = nullptr);
    ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QWidget *paramWidget) const override;
    void applyParamWidgetValue(QWidget *paramWidget) override;
    void clearParamWidget(QWidget *paramWidget) const override;

    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;

    [[nodiscard]] QString argsAsString() const override;
    void argsFromString(const QString &argsStr) override;

    [[nodiscard]] QString displayString() const override;

    static FilterAction *newAction();

    [[nodiscard]] QStringList sieveRequires() const override;
    [[nodiscard]] QString sieveCode() const override;

    [[nodiscard]] bool isEmpty() const override;
    [[nodiscard]] QString informationAboutNotValidAction() const override;

private:
    QString mValue;
};
}

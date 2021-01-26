/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONADDHEADER_H
#define MAILCOMMON_FILTERACTIONADDHEADER_H

#include "filteractionwithstringlist.h"

namespace MailCommon
{
//=============================================================================
// FilterActionAddHeader - add header
// Add a header with the given value.
//=============================================================================
class FilterActionAddHeader : public FilterActionWithStringList
{
    Q_OBJECT
public:
    explicit FilterActionAddHeader(QObject *parent = nullptr);
    ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QWidget *paramWidget) const override;
    void applyParamWidgetValue(QWidget *paramWidget) override;
    void clearParamWidget(QWidget *paramWidget) const override;

    SearchRule::RequiredPart requiredPart() const override;

    Q_REQUIRED_RESULT QString argsAsString() const override;
    void argsFromString(const QString &argsStr) override;

    Q_REQUIRED_RESULT QString displayString() const override;

    static FilterAction *newAction();

    Q_REQUIRED_RESULT QStringList sieveRequires() const override;
    Q_REQUIRED_RESULT QString sieveCode() const override;

    bool isEmpty() const override;
    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;

private:
    QString mValue;
};
}

#endif

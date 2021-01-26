/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONADDTOADDRESSBOOK_H
#define MAILCOMMON_FILTERACTIONADDTOADDRESSBOOK_H

#include "filteractionwithstringlist.h"

namespace MailCommon
{
//=============================================================================
// FilterActionAddToAddressBook
// - add email address from header to address book
//=============================================================================
class FilterActionAddToAddressBook : public FilterActionWithStringList
{
    Q_OBJECT
public:
    explicit FilterActionAddToAddressBook(QObject *parent = nullptr);
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    static FilterAction *newAction();

    SearchRule::RequiredPart requiredPart() const override;

    Q_REQUIRED_RESULT bool isEmpty() const override;

    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QWidget *paramWidget) const override;
    void applyParamWidgetValue(QWidget *paramWidget) override;
    void clearParamWidget(QWidget *paramWidget) const override;

    Q_REQUIRED_RESULT QString argsAsString() const override;
    void argsFromString(const QString &argsStr) override;

    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;

private:
    enum HeaderType { FromHeader, ToHeader, CcHeader, BccHeader, UnknownHeader };

    const QString mFromStr, mToStr, mCCStr, mBCCStr;
    HeaderType mHeaderType;
    Akonadi::Collection::Id mCollectionId;
    QString mCategory;
};
}

#endif

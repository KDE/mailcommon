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
// FilterActionAddToAddressBook
// - add email address from header to address book
//=============================================================================
class MAILCOMMON_TESTS_EXPORT FilterActionAddToAddressBook : public FilterActionWithStringList
{
    Q_OBJECT
public:
    explicit FilterActionAddToAddressBook(QObject *parent = nullptr);
    [[nodiscard]] ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    static FilterAction *newAction();

    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;

    [[nodiscard]] bool isEmpty() const override;

    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QWidget *paramWidget) const override;
    void applyParamWidgetValue(QWidget *paramWidget) override;
    void clearParamWidget(QWidget *paramWidget) const override;

    [[nodiscard]] QString argsAsString() const override;
    void argsFromString(const QString &argsStr) override;

    [[nodiscard]] QString informationAboutNotValidAction() const override;

private:
    enum HeaderType {
        FromHeader,
        ToHeader,
        CcHeader,
        BccHeader,
        UnknownHeader
    };

    const QString mFromStr, mToStr, mCCStr, mBCCStr;
    HeaderType mHeaderType;
    Akonadi::Collection::Id mCollectionId;
    QString mCategory;
};
}

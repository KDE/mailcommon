/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONREMOVEHEADER_H
#define MAILCOMMON_FILTERACTIONREMOVEHEADER_H

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
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;
    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QWidget *paramWidget) const override;
    Q_REQUIRED_RESULT QStringList sieveRequires() const override;
    Q_REQUIRED_RESULT QString sieveCode() const override;

    static FilterAction *newAction();
    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;
};
}

#endif

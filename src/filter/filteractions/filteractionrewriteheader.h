/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONREWRITEHEADER_H
#define MAILCOMMON_FILTERACTIONREWRITEHEADER_H

#include "filteractionwithstringlist.h"

#include <QRegularExpression>

namespace MailCommon
{
//=============================================================================
// FilterActionRewriteHeader - rewrite header
// Rewrite a header using a regexp.
//=============================================================================
class FilterActionRewriteHeader : public FilterActionWithStringList
{
    Q_OBJECT
public:
    explicit FilterActionRewriteHeader(QObject *parent = nullptr);
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;
    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QWidget *paramWidget) const override;
    void applyParamWidgetValue(QWidget *paramWidget) override;
    void clearParamWidget(QWidget *paramWidget) const override;

    Q_REQUIRED_RESULT QString argsAsString() const override;
    void argsFromString(const QString &argsStr) override;

    Q_REQUIRED_RESULT QString displayString() const override;

    static FilterAction *newAction();

    Q_REQUIRED_RESULT bool isEmpty() const override;
    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;

private:
    QRegularExpression mRegex;
    QString mReplacementString;
};
}

#endif

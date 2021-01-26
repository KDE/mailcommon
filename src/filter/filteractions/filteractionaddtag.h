/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONADDTAG_H
#define MAILCOMMON_FILTERACTIONADDTAG_H

#include "filteractionwithstringlist.h"

#include <QPointer>

class QComboBox;

namespace MailCommon
{
//=============================================================================
// FilterActionAddTag - append tag to message
// Appends a tag to messages
//=============================================================================
class FilterActionAddTag : public FilterAction
{
    Q_OBJECT
public:
    explicit FilterActionAddTag(QObject *parent = nullptr);
    ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    SearchRule::RequiredPart requiredPart() const override;

    static FilterAction *newAction();

    Q_REQUIRED_RESULT bool isEmpty() const override;

    void argsFromString(const QString &argsStr) override;
    Q_REQUIRED_RESULT QString argsAsString() const override;
    Q_REQUIRED_RESULT QString displayString() const override;
    Q_REQUIRED_RESULT bool argsFromStringInteractive(const QString &argsStr, const QString &filterName) override;

    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    void applyParamWidgetValue(QWidget *paramWidget) override;
    void setParamWidgetValue(QWidget *paramWidget) const override;
    void clearParamWidget(QWidget *paramWidget) const override;

    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;
private Q_SLOTS:
    void slotTagListingFinished();

private:
    void fillComboBox();
    mutable QMap<QUrl, QString> mList;
    QString mParameter;
    mutable QPointer<QComboBox> mComboBox;
};
}

#endif

/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteraction.h"
#include "mailcommon_private_export.h"
#include <QPointer>

class QComboBox;

namespace MailCommon
{
//=============================================================================
// FilterActionAddTag - append tag to message
// Appends a tag to messages
//=============================================================================
class MAILCOMMON_TESTS_EXPORT FilterActionAddTag : public FilterAction
{
    Q_OBJECT
public:
    explicit FilterActionAddTag(QObject *parent = nullptr);
    ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    SearchRule::RequiredPart requiredPart() const override;

    static FilterAction *newAction();

    [[nodiscard]] bool isEmpty() const override;

    void argsFromString(const QString &argsStr) override;
    [[nodiscard]] QString argsAsString() const override;
    [[nodiscard]] QString displayString() const override;
    [[nodiscard]] bool argsFromStringInteractive(const QString &argsStr, const QString &filterName) override;

    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;
    void applyParamWidgetValue(QWidget *paramWidget) override;
    void setParamWidgetValue(QWidget *paramWidget) const override;
    void clearParamWidget(QWidget *paramWidget) const override;

    [[nodiscard]] QString informationAboutNotValidAction() const override;

private:
    void slotTagListingFinished();

private:
    void fillComboBox();
    mutable QMap<QUrl, QString> mList;
    QString mParameter;
    mutable QPointer<QComboBox> mComboBox;
};
}

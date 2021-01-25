/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionwithstring.h"

#include <KLineEdit>

using namespace MailCommon;

FilterActionWithString::FilterActionWithString(const QString &name, const QString &label, QObject *parent)
    : FilterAction(name, label, parent)
{
}

bool FilterActionWithString::isEmpty() const
{
    return mParameter.trimmed().isEmpty();
}

QWidget *FilterActionWithString::createParamWidget(QWidget *parent) const
{
    auto lineEdit = new KLineEdit(parent);
    lineEdit->setClearButtonEnabled(true);
    lineEdit->setObjectName(QStringLiteral("lineedit"));
    lineEdit->setTrapReturnKey(true);
    lineEdit->setText(mParameter);

    connect(lineEdit, &KLineEdit::textChanged, this, &FilterActionWithString::filterActionModified);

    return lineEdit;
}

void FilterActionWithString::applyParamWidgetValue(QWidget *paramWidget)
{
    mParameter = static_cast<KLineEdit *>(paramWidget)->text();
}

void FilterActionWithString::setParamWidgetValue(QWidget *paramWidget) const
{
    static_cast<KLineEdit *>(paramWidget)->setText(mParameter);
}

void FilterActionWithString::clearParamWidget(QWidget *paramWidget) const
{
    static_cast<KLineEdit *>(paramWidget)->clear();
}

void FilterActionWithString::argsFromString(const QString &argsStr)
{
    mParameter = argsStr;
}

QString FilterActionWithString::argsAsString() const
{
    return mParameter;
}

QString FilterActionWithString::displayString() const
{
    return label() + QLatin1String(" \"") + argsAsString().toHtmlEscaped() + QLatin1String("\"");
}

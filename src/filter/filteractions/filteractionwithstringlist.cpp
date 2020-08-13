/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionwithstringlist.h"

#include <QComboBox>

using namespace MailCommon;

FilterActionWithStringList::FilterActionWithStringList(const QString &name, const QString &label, QObject *parent)
    : FilterActionWithString(name, label, parent)
{
}

QWidget *FilterActionWithStringList::createParamWidget(QWidget *parent) const
{
    auto comboBox = new QComboBox(parent);
    comboBox->setMinimumWidth(50);
    comboBox->setObjectName(QStringLiteral("combobox"));
    comboBox->setEditable(false);
    comboBox->addItems(mParameterList);
    setParamWidgetValue(comboBox);
    connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FilterActionWithStringList::filterActionModified);

    return comboBox;
}

void FilterActionWithStringList::applyParamWidgetValue(QWidget *paramWidget)
{
    mParameter = static_cast<QComboBox *>(paramWidget)->currentText();
}

void FilterActionWithStringList::setParamWidgetValue(QWidget *paramWidget) const
{
    const int index = mParameterList.indexOf(mParameter);
    static_cast<QComboBox *>(paramWidget)->setCurrentIndex(index >= 0 ? index : 0);
}

void FilterActionWithStringList::clearParamWidget(QWidget *paramWidget) const
{
    static_cast<QComboBox *>(paramWidget)->setCurrentIndex(0);
}

void FilterActionWithStringList::argsFromString(const QString &argsStr)
{
    int index = mParameterList.indexOf(argsStr);
    if (index < 0) {
        mParameterList.append(argsStr);
        index = mParameterList.count() - 1;
    }

    mParameter = mParameterList.at(index);
}

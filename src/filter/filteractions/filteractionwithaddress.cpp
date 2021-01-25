/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionwithaddress.h"

#include <Akonadi/Contact/EmailAddressRequester>

using namespace MailCommon;

FilterActionWithAddress::FilterActionWithAddress(const QString &name, const QString &label, QObject *parent)
    : FilterActionWithString(name, label, parent)
{
}

QWidget *FilterActionWithAddress::createParamWidget(QWidget *parent) const
{
    auto requester = new Akonadi::EmailAddressRequester(parent);
    requester->setText(mParameter);
    requester->setObjectName(QStringLiteral("emailaddressrequester"));
    requester->setText(mParameter);

    connect(requester, &Akonadi::EmailAddressRequester::textChanged, this, &FilterActionWithAddress::filterActionModified);

    return requester;
}

void FilterActionWithAddress::applyParamWidgetValue(QWidget *paramWidget)
{
    mParameter = qobject_cast<Akonadi::EmailAddressRequester *>(paramWidget)->text();
}

void FilterActionWithAddress::setParamWidgetValue(QWidget *paramWidget) const
{
    qobject_cast<Akonadi::EmailAddressRequester *>(paramWidget)->setText(mParameter);
}

void FilterActionWithAddress::clearParamWidget(QWidget *paramWidget) const
{
    qobject_cast<Akonadi::EmailAddressRequester *>(paramWidget)->clear();
}

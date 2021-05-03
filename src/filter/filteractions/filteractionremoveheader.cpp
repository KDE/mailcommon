/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionremoveheader.h"

#include <KComboBox>
#include <KLocalizedString>
#include <QLineEdit>

using namespace MailCommon;

FilterAction *FilterActionRemoveHeader::newAction()
{
    return new FilterActionRemoveHeader;
}

FilterActionRemoveHeader::FilterActionRemoveHeader(QObject *parent)
    : FilterActionWithStringList(QStringLiteral("remove header"), i18n("Remove Header"), parent)
{
    mParameterList << QString() << QStringLiteral("Reply-To") << QStringLiteral("Delivered-To") << QStringLiteral("X-KDE-PR-Message")
                   << QStringLiteral("X-KDE-PR-Package") << QStringLiteral("X-KDE-PR-Keywords");

    mParameter = mParameterList.at(0);
}

QWidget *FilterActionRemoveHeader::createParamWidget(QWidget *parent) const
{
    auto comboBox = new KComboBox(parent);
    comboBox->setEditable(true);
    comboBox->setMinimumWidth(50);
    comboBox->setInsertPolicy(QComboBox::InsertAtBottom);
    setParamWidgetValue(comboBox);
    connect(comboBox, QOverload<int>::of(&KComboBox::currentIndexChanged), this, &FilterActionRemoveHeader::filterActionModified);
    connect(comboBox->lineEdit(), &QLineEdit::textChanged, this, &FilterAction::filterActionModified);

    return comboBox;
}

FilterAction::ReturnCode FilterActionRemoveHeader::process(ItemContext &context, bool) const
{
    if (isEmpty()) {
        return ErrorButGoOn;
    }

    auto msg = context.item().payload<KMime::Message::Ptr>();
    const QByteArray param(mParameter.toLatin1());
    bool headerRemove = false;
    while (msg->removeHeader(param.constData())) {
        headerRemove = true;
    }
    if (headerRemove) {
        msg->assemble();
        context.setNeedsPayloadStore();
    }

    return GoOn;
}

SearchRule::RequiredPart FilterActionRemoveHeader::requiredPart() const
{
    return SearchRule::CompleteMessage;
}

void FilterActionRemoveHeader::setParamWidgetValue(QWidget *paramWidget) const
{
    const auto comboBox = qobject_cast<KComboBox *>(paramWidget);
    Q_ASSERT(comboBox);

    const int index = mParameterList.indexOf(mParameter);
    comboBox->clear();
    comboBox->addItems(mParameterList);
    if (index < 0) {
        comboBox->addItem(mParameter);
        comboBox->setCurrentIndex(comboBox->count() - 1);
    } else {
        comboBox->setCurrentIndex(index);
    }
}

QStringList FilterActionRemoveHeader::sieveRequires() const
{
    return QStringList() << QStringLiteral("editheader");
}

QString FilterActionRemoveHeader::sieveCode() const
{
    return QStringLiteral("deleteheader \"%1\";").arg(mParameter);
}

QString FilterActionRemoveHeader::informationAboutNotValidAction() const
{
    return i18n("Header name undefined.");
}

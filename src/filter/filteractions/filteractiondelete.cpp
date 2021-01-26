/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractiondelete.h"

#include <KColorScheme>
#include <KLocalizedString>

#include <QLabel>

using namespace MailCommon;

FilterActionDelete::FilterActionDelete(QObject *parent)
    : FilterActionWithNone(QStringLiteral("delete"), i18n("Delete Message"), parent)
{
}

FilterAction::ReturnCode FilterActionDelete::process(ItemContext &context, bool) const
{
    context.setDeleteItem();
    return GoOn;
}

SearchRule::RequiredPart FilterActionDelete::requiredPart() const
{
    return SearchRule::Envelope;
}

QWidget *FilterActionDelete::createParamWidget(QWidget *parent) const
{
    auto lab = new QLabel(parent);
    lab->setObjectName(QStringLiteral("label_delete"));
    QPalette pal = lab->palette();
    KColorScheme scheme(QPalette::Active, KColorScheme::View);
    pal.setColor(QPalette::WindowText, scheme.foreground(KColorScheme::NegativeText).color());
    lab->setPalette(pal);
    lab->setText(i18n("Be careful, mails will be removed."));
    return lab;
}

FilterAction *FilterActionDelete::newAction()
{
    return new FilterActionDelete;
}

QString FilterActionDelete::sieveCode() const
{
    return QStringLiteral("discard;");
}

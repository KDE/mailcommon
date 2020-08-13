/*
  SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "redirectwidget.h"
#include "kernel/mailkernel.h"

#include <QTreeView>
#include <MessageComposer/ComposerLineEdit>

#include <Akonadi/Contact/EmailAddressSelectionDialog>

#include <QIcon>
#include <QHBoxLayout>
#include <QPushButton>
#include <KLocalizedString>

using namespace MailCommon;

RedirectWidget::RedirectWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setSpacing(0);
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->setAlignment(Qt::AlignRight);

    mEdit = new MessageComposer::ComposerLineEdit(true);
    mEdit->setRecentAddressConfig(KernelIf->config().data());
    mEdit->setMinimumWidth(300);
    mEdit->setClearButtonEnabled(true);
    hbox->addWidget(mEdit);

    QPushButton *BtnTo = new QPushButton(this);
    BtnTo->setIcon(QIcon::fromTheme(QStringLiteral("help-contents")));
    BtnTo->setToolTip(i18n("Use the Address-Selection Dialog"));
    BtnTo->setWhatsThis(i18n("This button opens a separate dialog "
                             "where you can select recipients out "
                             "of all available addresses."));
    hbox->addWidget(BtnTo);
    connect(BtnTo, &QAbstractButton::clicked, this, &RedirectWidget::slotAddressSelection);

    connect(mEdit, &QLineEdit::textChanged, this, &RedirectWidget::addressChanged);

    setFocusPolicy(Qt::StrongFocus);
    setFocusProxy(mEdit);
}

RedirectWidget::~RedirectWidget()
{
}

QString RedirectWidget::resend()
{
    mResendStr = mEdit->text();
    return mResendStr;
}

void RedirectWidget::setFocus()
{
    mEdit->setFocus();
}

void RedirectWidget::slotAddressSelection()
{
    std::unique_ptr<Akonadi::EmailAddressSelectionDialog> dlg(
        new Akonadi::EmailAddressSelectionDialog(this));

    dlg->view()->view()->setSelectionMode(QAbstractItemView::MultiSelection);

    mResendStr = mEdit->text();

    if (dlg->exec() != QDialog::Rejected && dlg) {
        QStringList addresses;
        const Akonadi::EmailAddressSelection::List lstAddress = dlg->selectedAddresses();
        addresses.reserve(lstAddress.count());
        for (const Akonadi::EmailAddressSelection &selection : lstAddress) {
            addresses << selection.quotedEmail();
        }

        if (!mResendStr.isEmpty()) {
            addresses.prepend(mResendStr);
        }

        mEdit->setText(addresses.join(QLatin1String(", ")));
        mEdit->setModified(true);
    }
}

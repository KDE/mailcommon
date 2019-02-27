/*
  Copyright (c) 2015-2019 Montel Laurent <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.

*/

#include "redirectwidget.h"
#include "kernel/mailkernel.h"

#include <QTreeView>
#include <KIconLoader>
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
    BtnTo->setIconSize(QSize(KIconLoader::SizeSmall, KIconLoader::SizeSmall));
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

        mEdit->setText(addresses.join(QStringLiteral(", ")));
        mEdit->setModified(true);
    }
}

/*
  SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "redirectwidget.h"
#include "kernel/mailkernel.h"

#include <MessageComposer/ComposerLineEdit>
#include <QTreeView>

#include <Akonadi/EmailAddressSelectionDialog>

#include <KLocalizedString>
#include <QHBoxLayout>
#include <QIcon>
#include <QPushButton>

using namespace MailCommon;

RedirectWidget::RedirectWidget(QWidget *parent)
    : QWidget(parent)
    , mEdit(new MessageComposer::ComposerLineEdit(true, this))
{
    auto hbox = new QHBoxLayout(this);
    hbox->setSpacing(0);
    hbox->setContentsMargins({});
    hbox->setAlignment(Qt::AlignRight);

    mEdit->setRecentAddressConfig(KernelIf->config().data());
    mEdit->setMinimumWidth(300);
    mEdit->setClearButtonEnabled(true);
    hbox->addWidget(mEdit);

    auto BtnTo = new QPushButton(this);
    BtnTo->setIcon(QIcon::fromTheme(QStringLiteral("help-contents")));
    BtnTo->setToolTip(i18nc("@info:tooltip", "Use the Address-Selection Dialog"));
    BtnTo->setWhatsThis(
        i18n("This button opens a separate dialog "
             "where you can select recipients out "
             "of all available addresses."));
    hbox->addWidget(BtnTo);
    connect(BtnTo, &QAbstractButton::clicked, this, &RedirectWidget::slotAddressSelection);

    connect(mEdit, &QLineEdit::textChanged, this, &RedirectWidget::addressChanged);

    setFocusPolicy(Qt::StrongFocus);
    setFocusProxy(mEdit);
}

RedirectWidget::~RedirectWidget() = default;

QString RedirectWidget::resend()
{
    mResendStr = mEdit->text();
    return mResendStr;
}

void RedirectWidget::forceFocus()
{
    mEdit->setFocus();
}

void RedirectWidget::slotAddressSelection()
{
    std::unique_ptr<Akonadi::EmailAddressSelectionDialog> dlg(new Akonadi::EmailAddressSelectionDialog(this));

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

        mEdit->setText(addresses.join(QLatin1StringView(", ")));
        mEdit->setModified(true);
    }
}

#include "moc_redirectwidget.cpp"

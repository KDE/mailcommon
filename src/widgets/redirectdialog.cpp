/*
  Copyright (c) 2003 Andreas Gungl <a.gungl@gmx.de>
  Copyright (C) 2014-2018 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  In addition, as a special exception, the copyright holders give
  permission to link the code of this program with any edition of
  the Qt library by Trolltech AS, Norway (or with modified versions
  of Qt that use the same license as Qt), and distribute linked
  combinations including the two.  You must obey the GNU General
  Public License in all respects for all of the code used other than
  Qt.  If you modify this file, you may extend this exception to
  your version of the file, but you are not obligated to do so.  If
  you do not wish to do so, delete this exception statement from
  your version.
*/

#include "redirectdialog.h"
#include "kernel/mailkernel.h"

#include "redirectwidget.h"

#include <KIdentityManagement/Identity>
#include <KIdentityManagement/IdentityCombo>
#include <KIdentityManagement/IdentityManager>

#include <MailTransport/TransportComboBox>

#include <KLocalizedString>
#include <KMessageBox>

#include <QLabel>
#include <QPushButton>
#include <QTreeView>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <KGuiItem>
#include <QVBoxLayout>

using namespace MailCommon;

class Q_DECL_HIDDEN RedirectDialog::Private
{
public:
    Private(RedirectDialog *qq, RedirectDialog::SendMode mode)
        : q(qq)
        , mEditTo(nullptr)
        , mEditCc(nullptr)
        , mEditBcc(nullptr)
        , mSendMode(mode)
        , mComboboxIdentity(nullptr)
        , mTransportCombobox(nullptr)
        , mUser1Button(nullptr)
        , mUser2Button(nullptr)
    {
    }

    enum TypeAddress {
        ResendTo,
        ResendCc,
        ResendBcc
    };

    void slotUser1();
    void slotUser2();
    void slotAddressChanged(const QString &);
    QString redirectLabelType(TypeAddress type) const;
    RedirectDialog *q = nullptr;
    RedirectWidget *mEditTo = nullptr;
    RedirectWidget *mEditCc = nullptr;
    RedirectWidget *mEditBcc = nullptr;

    RedirectDialog::SendMode mSendMode;
    KIdentityManagement::IdentityCombo *mComboboxIdentity = nullptr;
    MailTransport::TransportComboBox *mTransportCombobox = nullptr;
    QPushButton *mUser1Button = nullptr;
    QPushButton *mUser2Button = nullptr;
};

QString RedirectDialog::Private::redirectLabelType(TypeAddress type) const
{
    QString label;
    switch (type) {
    case ResendTo:
        label = i18n("Resend-To:");
        break;
    case ResendCc:
        label = i18n("Resend-Cc:");
        break;
    case ResendBcc:
        label = i18n("Resend-Bcc:");
        break;
    }
    return label;
}

void RedirectDialog::Private::slotUser1()
{
    mSendMode = RedirectDialog::SendNow;
    q->accept();
}

void RedirectDialog::Private::slotUser2()
{
    mSendMode = RedirectDialog::SendLater;
    q->accept();
}

void RedirectDialog::Private::slotAddressChanged(const QString &text)
{
    const bool textIsNotEmpty(!text.trimmed().isEmpty());
    mUser1Button->setEnabled(textIsNotEmpty);
    mUser2Button->setEnabled(textIsNotEmpty);
}

RedirectDialog::RedirectDialog(SendMode mode, QWidget *parent)
    : QDialog(parent)
    , d(new Private(this, mode))
{
    setWindowTitle(i18n("Redirect Message"));
    QVBoxLayout *topLayout = new QVBoxLayout(this);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel, this);
    d->mUser1Button = new QPushButton(this);
    buttonBox->addButton(d->mUser1Button, QDialogButtonBox::ActionRole);
    d->mUser2Button = new QPushButton(this);
    buttonBox->addButton(d->mUser2Button, QDialogButtonBox::ActionRole);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &RedirectDialog::reject);
    if (mode == SendNow) {
        d->mUser1Button->setDefault(true);
    } else {
        d->mUser2Button->setDefault(true);
    }

    QWidget *mainWidget = new QWidget;
    topLayout->addWidget(mainWidget);
    topLayout->addWidget(buttonBox);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainWidget->setLayout(mainLayout);
    mainLayout->setMargin(0);
    QLabel *LabelTo = new QLabel(i18n("Select the recipient addresses to redirect to:"));
    mainLayout->addWidget(LabelTo);

    QFormLayout *formLayout = new QFormLayout;
    // Combo boxes look better when they are the same size as other full width fields.
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    // This is set to zero to avoid double margins between the
    // three address fields, due to their addresss selection buttons.
    formLayout->setVerticalSpacing(0);
    mainLayout->addLayout(formLayout);

    d->mEditTo = new RedirectWidget;
    formLayout->addRow(d->redirectLabelType(RedirectDialog::Private::ResendTo), d->mEditTo);

    connect(d->mEditTo, &RedirectWidget::addressChanged, this, [this](const QString &str) {
        d->slotAddressChanged(str);
    });

    d->mEditCc = new RedirectWidget;
    formLayout->addRow(d->redirectLabelType(RedirectDialog::Private::ResendCc), d->mEditCc);
    d->mEditBcc = new RedirectWidget;
    formLayout->addRow(d->redirectLabelType(RedirectDialog::Private::ResendBcc), d->mEditBcc);
    d->mEditTo->setFocus();

    // Because the form layout vertical spacing was set to zero above,
    // it is necessary to explicitly add spacing between the combo boxes.
    // The layout default spacing is copied from the main layout.
    formLayout->addItem(new QSpacerItem(1, mainLayout->spacing(), QSizePolicy::Fixed, QSizePolicy::Fixed));
    d->mComboboxIdentity = new KIdentityManagement::IdentityCombo(KernelIf->identityManager());
    formLayout->addRow(i18n("Identity:"), d->mComboboxIdentity);

    formLayout->addItem(new QSpacerItem(1, mainLayout->spacing(), QSizePolicy::Fixed, QSizePolicy::Fixed));
    d->mTransportCombobox = new MailTransport::TransportComboBox;
    formLayout->addRow(i18n("Transport:"), d->mTransportCombobox);

    KGuiItem::assign(d->mUser1Button, KGuiItem(i18n("&Send Now"), QIcon::fromTheme("mail-send")));
    KGuiItem::assign(d->mUser2Button, KGuiItem(i18n("Send &Later"), QIcon::fromTheme("mail-queue")));
    connect(d->mUser1Button, &QPushButton::clicked, this, [this]() {
        d->slotUser1();
    });
    connect(d->mUser2Button, &QPushButton::clicked, this, [this]() {
        d->slotUser2();
    });

    d->mUser1Button->setEnabled(false);
    d->mUser2Button->setEnabled(false);
}

RedirectDialog::~RedirectDialog()
{
    delete d;
}

QString RedirectDialog::to() const
{
    return d->mEditTo->resend();
}

QString RedirectDialog::cc() const
{
    return d->mEditCc->resend();
}

QString RedirectDialog::bcc() const
{
    return d->mEditBcc->resend();
}

RedirectDialog::SendMode RedirectDialog::sendMode() const
{
    return d->mSendMode;
}

int RedirectDialog::transportId() const
{
    return d->mTransportCombobox->currentTransportId();
}

int RedirectDialog::identity() const
{
    return static_cast<int>(d->mComboboxIdentity->currentIdentity());
}

void RedirectDialog::accept()
{
    const QString editTo = d->mEditTo->resend();
    if (editTo.isEmpty()) {
        KMessageBox::sorry(
            this,
            i18n("You cannot redirect the message without an address."),
            i18n("Empty Redirection Address"));
    } else {
        done(QDialog::Accepted);
    }
}

#include "moc_redirectdialog.cpp"

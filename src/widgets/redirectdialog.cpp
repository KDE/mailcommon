/*
  SPDX-FileCopyrightText: 2003 Andreas Gungl <a.gungl@gmx.de>
  SPDX-FileCopyrightText: 2014-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "redirectdialog.h"
#include "kernel/mailkernel.h"

#include "redirectwidget.h"

#include <KIdentityManagementCore/Identity>
#include <KIdentityManagementWidgets/IdentityCombo>

#include <MailTransport/TransportComboBox>

#include <KLocalizedString>
#include <KMessageBox>

#include <KGuiItem>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QTreeView>
#include <QVBoxLayout>

using namespace MailCommon;

class Q_DECL_HIDDEN RedirectDialog::RedirectDialogPrivate
{
public:
    RedirectDialogPrivate(RedirectDialog *qq, RedirectDialog::SendMode mode)
        : q(qq)
        , mSendMode(mode)
    {
    }

    enum TypeAddress {
        ResendTo,
        ResendCc,
        ResendBcc,
    };

    void slotUser1();
    void slotUser2();
    void slotAddressChanged(const QString &);
    [[nodiscard]] QString redirectLabelType(TypeAddress type) const;
    RedirectDialog *const q = nullptr;
    RedirectWidget *mEditTo = nullptr;
    RedirectWidget *mEditCc = nullptr;
    RedirectWidget *mEditBcc = nullptr;

    RedirectDialog::SendMode mSendMode;
    KIdentityManagementWidgets::IdentityCombo *mComboboxIdentity = nullptr;
    MailTransport::TransportComboBox *mTransportCombobox = nullptr;
    QPushButton *mUser1Button = nullptr;
    QPushButton *mUser2Button = nullptr;
};

QString RedirectDialog::RedirectDialogPrivate::redirectLabelType(TypeAddress type) const
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

void RedirectDialog::RedirectDialogPrivate::slotUser1()
{
    mSendMode = RedirectDialog::SendNow;
    q->accept();
}

void RedirectDialog::RedirectDialogPrivate::slotUser2()
{
    mSendMode = RedirectDialog::SendLater;
    q->accept();
}

void RedirectDialog::RedirectDialogPrivate::slotAddressChanged(const QString &text)
{
    const bool textIsNotEmpty(!text.trimmed().isEmpty());
    mUser1Button->setEnabled(textIsNotEmpty);
    mUser2Button->setEnabled(textIsNotEmpty);
}

RedirectDialog::RedirectDialog(SendMode mode, QWidget *parent)
    : QDialog(parent)
    , d(new RedirectDialogPrivate(this, mode))
{
    setWindowTitle(i18nc("@title:window", "Redirect Message"));
    auto topLayout = new QVBoxLayout(this);
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel, this);
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

    auto mainWidget = new QWidget(this);
    topLayout->addWidget(mainWidget);
    topLayout->addWidget(buttonBox);

    auto mainLayout = new QVBoxLayout;
    mainWidget->setLayout(mainLayout);
    mainLayout->setContentsMargins({});
    auto LabelTo = new QLabel(i18nc("@label:textbox", "Select the recipient addresses to redirect to:"), this);
    mainLayout->addWidget(LabelTo);

    auto formLayout = new QFormLayout;
    // Combo boxes look better when they are the same size as other full width fields.
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    // This is set to zero to avoid double margins between the
    // three address fields, due to their address selection buttons.
    formLayout->setVerticalSpacing(0);
    mainLayout->addLayout(formLayout);

    d->mEditTo = new RedirectWidget(this);
    formLayout->addRow(d->redirectLabelType(RedirectDialog::RedirectDialogPrivate::ResendTo), d->mEditTo);

    connect(d->mEditTo, &RedirectWidget::addressChanged, this, [this](const QString &str) {
        d->slotAddressChanged(str);
    });

    d->mEditCc = new RedirectWidget(this);
    formLayout->addRow(d->redirectLabelType(RedirectDialog::RedirectDialogPrivate::ResendCc), d->mEditCc);
    d->mEditBcc = new RedirectWidget(this);
    formLayout->addRow(d->redirectLabelType(RedirectDialog::RedirectDialogPrivate::ResendBcc), d->mEditBcc);
    d->mEditTo->forceFocus();

    // Because the form layout vertical spacing was set to zero above,
    // it is necessary to explicitly add spacing between the combo boxes.
    // The layout default spacing is copied from the main layout.
    formLayout->addItem(new QSpacerItem(1, mainLayout->spacing(), QSizePolicy::Fixed, QSizePolicy::Fixed));
    d->mComboboxIdentity = new KIdentityManagementWidgets::IdentityCombo(KernelIf->identityManager());
    formLayout->addRow(i18n("Identity:"), d->mComboboxIdentity);

    formLayout->addItem(new QSpacerItem(1, mainLayout->spacing(), QSizePolicy::Fixed, QSizePolicy::Fixed));
    d->mTransportCombobox = new MailTransport::TransportComboBox;
    formLayout->addRow(i18n("Transport:"), d->mTransportCombobox);

    KGuiItem::assign(d->mUser1Button, KGuiItem(i18nc("@action:button", "&Send Now"), QIcon::fromTheme(QStringLiteral("mail-send"))));
    KGuiItem::assign(d->mUser2Button, KGuiItem(i18nc("@action:button", "Send &Later"), QIcon::fromTheme(QStringLiteral("mail-queue"))));
    connect(d->mUser1Button, &QPushButton::clicked, this, [this]() {
        d->slotUser1();
    });
    connect(d->mUser2Button, &QPushButton::clicked, this, [this]() {
        d->slotUser2();
    });

    d->mUser1Button->setEnabled(false);
    d->mUser2Button->setEnabled(false);
}

RedirectDialog::~RedirectDialog() = default;

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
        KMessageBox::error(this, i18n("You cannot redirect the message without an address."), i18nc("@title:window", "Empty Redirection Address"));
    } else {
        done(QDialog::Accepted);
    }
}

#include "moc_redirectdialog.cpp"

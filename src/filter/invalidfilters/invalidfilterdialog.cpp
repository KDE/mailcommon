/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "invalidfilterdialog.h"
#include "invalidfilterwidget.h"
#include "invalidfilterinfowidget.h"
#include <KConfigGroup>
#include <KSharedConfig>
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>

using namespace MailCommon;

InvalidFilterDialog::InvalidFilterDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Invalid Filters"));
    setWindowIcon(QIcon::fromTheme(QStringLiteral("kmail")));
    auto *mainLayout = new QVBoxLayout(this);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    okButton->setDefault(true);
    setModal(true);
    okButton->setText(i18n("Discard"));

    QWidget *w = new QWidget(this);
    auto *vbox = new QVBoxLayout;
    vbox->setContentsMargins(0, 0, 0, 0);
    w->setLayout(vbox);
    mInvalidFilterWidget = new InvalidFilterWidget(this);
    mInvalidFilterWidget->setObjectName(QStringLiteral("invalid_filter_widget"));
    vbox->addWidget(mInvalidFilterWidget);

    mInvalidFilterInfoWidget = new InvalidFilterInfoWidget(this);
    mInvalidFilterInfoWidget->setObjectName(QStringLiteral("invalid_filter_infowidget"));
    vbox->addWidget(mInvalidFilterInfoWidget);
    connect(mInvalidFilterWidget, &InvalidFilterWidget::showDetails, mInvalidFilterInfoWidget, &InvalidFilterInfoWidget::slotShowDetails);
    connect(mInvalidFilterWidget, &InvalidFilterWidget::hideInformationWidget, mInvalidFilterInfoWidget, &KMessageWidget::animatedHide);
    mainLayout->addWidget(w);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

InvalidFilterDialog::~InvalidFilterDialog()
{
    writeConfig();
}

void InvalidFilterDialog::setInvalidFilters(const QVector<InvalidFilterInfo> &lst)
{
    mInvalidFilterWidget->setInvalidFilters(lst);
}

void InvalidFilterDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "InvalidFilterDialog");
    group.writeEntry("Size", size());
}

void InvalidFilterDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "InvalidFilterDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 500));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

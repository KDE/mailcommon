/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "invalidfilterdialog.h"
#include "invalidfilterinfowidget.h"
#include "invalidfilterwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

using namespace MailCommon;
namespace
{
static const char myInvalidFilterDialogName[] = "InvalidFilterDialog";
}
InvalidFilterDialog::InvalidFilterDialog(QWidget *parent)
    : QDialog(parent)
    , mInvalidFilterWidget(new InvalidFilterWidget(this))
    , mInvalidFilterInfoWidget(new InvalidFilterInfoWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Invalid Filters"));
    setWindowIcon(QIcon::fromTheme(QStringLiteral("kmail")));
    auto mainLayout = new QVBoxLayout(this);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    okButton->setDefault(true);
    setModal(true);
    okButton->setText(i18n("Discard"));

    auto w = new QWidget(this);
    auto vbox = new QVBoxLayout;
    vbox->setContentsMargins({});
    w->setLayout(vbox);
    mInvalidFilterWidget->setObjectName(QLatin1StringView("invalid_filter_widget"));
    vbox->addWidget(mInvalidFilterWidget);

    mInvalidFilterInfoWidget->setObjectName(QLatin1StringView("invalid_filter_infowidget"));
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

void InvalidFilterDialog::setInvalidFilters(const QList<InvalidFilterInfo> &lst)
{
    mInvalidFilterWidget->setInvalidFilters(lst);
}

void InvalidFilterDialog::readConfig()
{
#if TEXTADDONSWIDGETS_VERSION >= QT_VERSION_CHECK(2, 1, 49)
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myInvalidFilterDialogName), QSize(400, 500));
#else
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 500));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myInvalidFilterDialogName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
#endif
}

void InvalidFilterDialog::writeConfig()
{
#if TEXTADDONSWIDGETS_VERSION < QT_VERSION_CHECK(2, 1, 49)
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myInvalidFilterDialogName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
#endif
}

#include "moc_invalidfilterdialog.cpp"

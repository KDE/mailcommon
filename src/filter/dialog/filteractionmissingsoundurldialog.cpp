/*
   SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingsoundurldialog.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KUrlRequester>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

using namespace MailCommon;
namespace
{
static const char myFilterActionMissingSoundUrlDialogGroupName[] = "FilterActionMissingSoundUrlDialog";
}

FilterActionMissingSoundUrlDialog::FilterActionMissingSoundUrlDialog(const QString &filtername, const QString &argStr, QWidget *parent)
    : QDialog(parent)
    , mUrlWidget(new KUrlRequester(this))
{
    setModal(true);
    setWindowTitle(i18nc("@title:window", "Select sound"));

    auto mainLayout = new QVBoxLayout(this);

    auto label = new QLabel(i18n("Sound file was \"%1\".", argStr));
    label->setObjectName(QStringLiteral("oldlabel"));
    mainLayout->addWidget(label);

    label = new QLabel(this);
    label->setObjectName(QStringLiteral("selectlabel"));
    label->setText(
        i18n("Sound file is missing. "
             "Please select a sound to use with filter \"%1\"",
             filtername));
    label->setWordWrap(true);
    mainLayout->addWidget(label);
    mUrlWidget->setObjectName(QStringLiteral("urlwidget"));
    mainLayout->addWidget(mUrlWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &FilterActionMissingSoundUrlDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterActionMissingSoundUrlDialog::reject);

    mainLayout->addWidget(buttonBox);
    readConfig();
}

FilterActionMissingSoundUrlDialog::~FilterActionMissingSoundUrlDialog()
{
    writeConfig();
}

QString FilterActionMissingSoundUrlDialog::soundUrl() const
{
    return mUrlWidget->url().path();
}

void FilterActionMissingSoundUrlDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(500, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myFilterActionMissingSoundUrlDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void FilterActionMissingSoundUrlDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myFilterActionMissingSoundUrlDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

#include "moc_filteractionmissingsoundurldialog.cpp"

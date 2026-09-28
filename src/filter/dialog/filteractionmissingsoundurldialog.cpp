/*
   SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingsoundurldialog.h"

#include <KLocalizedString>
#include <KUrlRequester>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

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

    auto label = new QLabel(i18nc("@label:textbox", "Sound file was \"%1\".", argStr));
    label->setObjectName(QLatin1StringView("oldlabel"));
    mainLayout->addWidget(label);

    label = new QLabel(this);
    label->setObjectName(QLatin1StringView("selectlabel"));
    label->setText(
        i18n("Sound file is missing. "
             "Please select a sound to use with filter \"%1\"",
             filtername));
    label->setWordWrap(true);
    mainLayout->addWidget(label);
    mUrlWidget->setObjectName(QLatin1StringView("urlwidget"));
    mainLayout->addWidget(mUrlWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QLatin1StringView("buttonbox"));
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &FilterActionMissingSoundUrlDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterActionMissingSoundUrlDialog::reject);

    mainLayout->addWidget(buttonBox);
    readConfig();
}

FilterActionMissingSoundUrlDialog::~FilterActionMissingSoundUrlDialog() = default;

QString FilterActionMissingSoundUrlDialog::soundUrl() const
{
    return mUrlWidget->url().path();
}

void FilterActionMissingSoundUrlDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myFilterActionMissingSoundUrlDialogGroupName), QSize(500, 300));
}

#include "moc_filteractionmissingsoundurldialog.cpp"

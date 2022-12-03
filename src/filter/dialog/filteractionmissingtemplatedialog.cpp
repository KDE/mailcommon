/*
   SPDX-FileCopyrightText: 2016-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingtemplatedialog.h"

#include <KSharedConfig>

#include <KConfigGroup>
#include <KLocalizedString>
#include <QComboBox>

#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

using namespace MailCommon;
namespace
{
static const char myFilterActionMissingTemplateDialogGroupName[] = "FilterActionMissingTemplateDialog";
}
FilterActionMissingTemplateDialog::FilterActionMissingTemplateDialog(const QStringList &templateList, const QString &filtername, QWidget *parent)
    : QDialog(parent)
    , mComboBoxTemplate(new QComboBox(this))
{
    setModal(true);
    setWindowTitle(i18nc("@title:window", "Select Template"));
    auto mainLayout = new QVBoxLayout(this);

    auto label = new QLabel(this);
    label->setObjectName(QStringLiteral("label"));
    label->setText(
        i18n("Filter template is missing. "
             "Please select a template to use with filter \"%1\"",
             filtername));
    label->setWordWrap(true);
    mainLayout->addWidget(label);
    mComboBoxTemplate->setObjectName(QStringLiteral("comboboxtemplate"));
    mComboBoxTemplate->addItems(templateList);
    mainLayout->addWidget(mComboBoxTemplate);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &FilterActionMissingTemplateDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterActionMissingTemplateDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

FilterActionMissingTemplateDialog::~FilterActionMissingTemplateDialog()
{
    writeConfig();
}

void FilterActionMissingTemplateDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(500, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), myFilterActionMissingTemplateDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void FilterActionMissingTemplateDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myFilterActionMissingTemplateDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

QString FilterActionMissingTemplateDialog::selectedTemplate() const
{
    if (mComboBoxTemplate->currentIndex() == 0) {
        return {};
    } else {
        return mComboBoxTemplate->currentText();
    }
}

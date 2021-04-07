/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingtemplatedialog.h"

#include <KSharedConfig>

#include <KConfigGroup>
#include <KLocalizedString>
#include <QComboBox>

#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace MailCommon;

FilterActionMissingTemplateDialog::FilterActionMissingTemplateDialog(const QStringList &templateList, const QString &filtername, QWidget *parent)
    : QDialog(parent)
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
    mComboBoxTemplate = new QComboBox(this);
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
    KConfigGroup group(KSharedConfig::openStateConfig(), "FilterActionMissingTemplateDialog");

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void FilterActionMissingTemplateDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "FilterActionMissingTemplateDialog");
    group.writeEntry("Size", size());
}

QString FilterActionMissingTemplateDialog::selectedTemplate() const
{
    if (mComboBoxTemplate->currentIndex() == 0) {
        return QString();
    } else {
        return mComboBoxTemplate->currentText();
    }
}

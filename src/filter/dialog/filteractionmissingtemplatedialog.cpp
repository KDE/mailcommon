/*
   Copyright (C) 2016 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "filteractionmissingtemplatedialog.h"

#include <KSharedConfig>

#include <KComboBox>
#include <KConfigGroup>
#include <KLocalizedString>

#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

using namespace MailCommon;

FilterActionMissingTemplateDialog::FilterActionMissingTemplateDialog(
    const QStringList &templateList, const QString &filtername, QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(i18n("Select Template"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *label = new QLabel(this);
    label->setObjectName(QStringLiteral("label"));
    label->setText(i18n("Filter template is missing. "
                        "Please select a template to use with filter \"%1\"",
                        filtername));
    label->setWordWrap(true);
    mainLayout->addWidget(label);
    mComboBoxTemplate = new KComboBox(this);
    mComboBoxTemplate->setObjectName(QStringLiteral("comboboxtemplate"));
    mComboBoxTemplate->addItems(templateList);
    mainLayout->addWidget(mComboBoxTemplate);


    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &FilterActionMissingTemplateDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterActionMissingTemplateDialog::reject);
    mainLayout->addWidget(buttonBox);
    okButton->setDefault(true);
    readConfig();
}

FilterActionMissingTemplateDialog::~FilterActionMissingTemplateDialog()
{
    writeConfig();
}

void FilterActionMissingTemplateDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "FilterActionMissingTemplateDialog");

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void FilterActionMissingTemplateDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "FilterActionMissingTemplateDialog");
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


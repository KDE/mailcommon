/*
  Copyright (C) 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.net,
    Author: Tobias Koenig <tokoe@kdab.com>

  Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
*/

#include "snippetvariabledialog.h"

#include <KLocalizedString>
#include <QMap>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <KConfigGroup>
#include <KSharedConfig>

#include <KPIMTextEdit/PlainTextEditorWidget>

using namespace MailCommon;
namespace {
static const char myConfigGroupName[] = "SnippetVariableDialog";
}
SnippetVariableDialog::SnippetVariableDialog(const QString &variableName, QMap<QString, QString> *variables, QWidget *parent)
    : QDialog(parent)
    , mVariableName(variableName)
    , mVariables(variables)
{
    setWindowTitle(i18nc("@title:window", "Enter Values for Variables"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *label = new QLabel(i18n("Enter the replacement values for '%1':", variableName), this);
    mainLayout->addWidget(label);

    mVariableValueText = new KPIMTextEdit::PlainTextEditorWidget(this);
    mainLayout->addWidget(mVariableValueText);

    mSaveVariable = new QCheckBox(i18n("Make value &default"), this);
    mSaveVariable->setChecked(false);
    mSaveVariable->setToolTip(
        i18nc("@info:tooltip",
              "Enable this to save the value entered to the right "
              "as the default value for this variable"));
    mSaveVariable->setWhatsThis(
        i18nc("@info:whatsthis",
              "If you enable this option, the value entered to the right will be saved. "
              "If you use the same variable later, even in another snippet, the value entered "
              "to the right will be the default value for that variable."));
    mainLayout->addWidget(mSaveVariable);

    if (mVariables->contains(variableName)) {
        mSaveVariable->setChecked(true);
        mVariableValueText->setPlainText(mVariables->value(variableName));
    }
    mVariableValueText->setFocus();

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SnippetVariableDialog::slotAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SnippetVariableDialog::reject);

    mainLayout->addWidget(buttonBox);
    readConfig();
}

SnippetVariableDialog::~SnippetVariableDialog()
{
    writeConfig();
}

QString SnippetVariableDialog::variableValue() const
{
    return mVariableValueText->toPlainText();
}

bool SnippetVariableDialog::saveVariableIsChecked() const
{
    return mSaveVariable->isChecked();
}

void SnippetVariableDialog::slotAccepted()
{
    if (mSaveVariable->isChecked()) {
        mVariables->insert(mVariableName, mVariableValueText->toPlainText());
    } else {
        mVariables->remove(mVariableName);
    }

    accept();
}

void SnippetVariableDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}

void SnippetVariableDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(300, 350));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

/*
  SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>
  SPDX-FileContributor: Tobias Koenig <tokoe@kdab.com>

  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "snippetvariabledialog.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QMap>
#include <QPushButton>
#include <QVBoxLayout>

#include <KPIMTextEdit/PlainTextEditorWidget>

using namespace MailCommon;
namespace
{
static const char myConfigGroupName[] = "SnippetVariableDialog";
}
SnippetVariableDialog::SnippetVariableDialog(const QString &variableName, QMap<QString, QString> *variables, QWidget *parent)
    : QDialog(parent)
    , mVariableName(variableName)
    , mVariables(variables)
{
    setWindowTitle(i18nc("@title:window", "Enter Values for Variables"));
    auto mainLayout = new QVBoxLayout(this);

    auto label = new QLabel(i18n("Enter the replacement values for '%1':", variableName), this);
    mainLayout->addWidget(label);

    mVariableValueText = new KPIMTextEdit::PlainTextEditorWidget(this);
    mainLayout->addWidget(mVariableValueText);

    mSaveVariable = new QCheckBox(i18n("Make value &default"), this);
    mSaveVariable->setChecked(false);
    mSaveVariable->setToolTip(i18nc("@info:tooltip",
                                    "Enable this to save the value entered to the right "
                                    "as the default value for this variable"));
    mSaveVariable->setWhatsThis(i18nc("@info:whatsthis",
                                      "If you enable this option, the value entered to the right will be saved. "
                                      "If you use the same variable later, even in another snippet, the value entered "
                                      "to the right will be the default value for that variable."));
    mainLayout->addWidget(mSaveVariable);

    if (mVariables->contains(variableName)) {
        mSaveVariable->setChecked(true);
        mVariableValueText->setPlainText(mVariables->value(variableName));
    }
    mVariableValueText->setFocus();

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
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
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}

void SnippetVariableDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(300, 350));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filterconverttosieveresultdialog.h"
#include "filterconverttosievepurposemenuwidget.h"
#include <KPIMTextEdit/PlainTextEditor>
#include <KPIMTextEdit/PlainTextEditorWidget>
#include <PimCommon/PimUtil>

#include <KLocalizedString>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>

#include <KConfigGroup>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

using namespace MailCommon;

FilterConvertToSieveResultDialog::FilterConvertToSieveResultDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Convert to Sieve Script"));
    auto topLayout = new QVBoxLayout(this);
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    auto saveButton = new QPushButton(this);
    buttonBox->addButton(saveButton, QDialogButtonBox::ActionRole);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterConvertToSieveResultDialog::reject);
    saveButton->setText(i18n("Save..."));
    saveButton->setObjectName(QStringLiteral("savebutton"));
    saveButton->setDefault(true);
    setModal(true);
    connect(saveButton, &QPushButton::clicked, this, &FilterConvertToSieveResultDialog::slotSave);

    mEditor = new KPIMTextEdit::PlainTextEditorWidget;
    mEditor->editor()->setSpellCheckingSupport(false);
    mEditor->setObjectName(QStringLiteral("editor"));
    auto syntaxHighlighter = new KSyntaxHighlighting::SyntaxHighlighter(mEditor->editor()->document());
    syntaxHighlighter->setDefinition(mSyntaxRepo.definitionForName(QStringLiteral("Sieve")));
    syntaxHighlighter->setTheme((palette().color(QPalette::Base).lightness() < 128) ? mSyntaxRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                                                                                    : mSyntaxRepo.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
    topLayout->addWidget(mEditor);
    topLayout->addWidget(buttonBox);

    auto purposeMenu = new FilterconverttosievePurposeMenuWidget(this, this);
    auto shareButton = new QPushButton(i18n("Share..."), this);
    shareButton->setMenu(purposeMenu->menu());
    shareButton->setIcon(QIcon::fromTheme(QStringLiteral("document-share")));
    purposeMenu->setEditorWidget(mEditor->editor());
    buttonBox->addButton(shareButton, QDialogButtonBox::ActionRole);

    readConfig();
}

FilterConvertToSieveResultDialog::~FilterConvertToSieveResultDialog()
{
    writeConfig();
}

void FilterConvertToSieveResultDialog::slotSave()
{
    const QString filter = i18n("Sieve Files (*.siv);;All Files (*)");
    PimCommon::Util::saveTextAs(mEditor->editor()->toPlainText(), filter, this, QUrl(), i18nc("@title:window", "Convert to Script Sieve"));
}

void FilterConvertToSieveResultDialog::setCode(const QString &code)
{
    mEditor->editor()->setPlainText(code);
}

static const char myConfigGroupName[] = "FilterConvertToSieveResultDialog";

void FilterConvertToSieveResultDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigGroupName);

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void FilterConvertToSieveResultDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
    group.sync();
}

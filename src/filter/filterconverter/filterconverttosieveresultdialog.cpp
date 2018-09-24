/*
  Copyright (c) 2013-2018 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "filterconverttosieveresultdialog.h"
#include "kpimtextedit/plaintexteditor.h"
#include "kpimtextedit/plaintexteditorwidget.h"
#include <PimCommon/PimUtil>

#ifdef KF5_USE_PURPOSE
#include <Purpose/AlternativesModel>
#include <PurposeWidgets/Menu>
#include <QJsonArray>
#endif


#include <KLocalizedString>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>

#include <KSharedConfig>
#include <KMessageBox>
#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTemporaryFile>

using namespace MailCommon;

FilterConvertToSieveResultDialog::FilterConvertToSieveResultDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Convert to Sieve Script"));
    QVBoxLayout *topLayout = new QVBoxLayout(this);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    QPushButton *saveButton = new QPushButton(this);
    buttonBox->addButton(saveButton, QDialogButtonBox::ActionRole);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterConvertToSieveResultDialog::reject);
    saveButton->setText(i18n("Save..."));
    saveButton->setObjectName(QStringLiteral("savebutton"));
    saveButton->setDefault(true);
    setModal(true);
    connect(saveButton, &QPushButton::clicked, this, &FilterConvertToSieveResultDialog::slotSave);

#ifdef KF5_USE_PURPOSE
    QPushButton *mShareButton = new QPushButton(i18n("Share..."), this);
    mShareMenu = new Purpose::Menu(this);
    mShareMenu->model()->setPluginType(QStringLiteral("Export"));
    connect(mShareMenu, &Purpose::Menu::aboutToShow, this, &FilterConvertToSieveResultDialog::slotInitializeShareMenu);
    mShareButton->setMenu(mShareMenu);
    mShareButton->setIcon( QIcon::fromTheme(QStringLiteral("document-share")));
    connect(mShareMenu, &Purpose::Menu::finished, this, &FilterConvertToSieveResultDialog::slotShareActionFinished);
    buttonBox->addButton(mShareButton, QDialogButtonBox::ActionRole);
#endif


    mEditor = new KPIMTextEdit::PlainTextEditorWidget;
    mEditor->editor()->setSpellCheckingSupport(false);
    mEditor->setObjectName(QStringLiteral("editor"));
    auto syntaxHighlighter = new KSyntaxHighlighting::SyntaxHighlighter(mEditor->editor()->document());
    syntaxHighlighter->setDefinition(mSyntaxRepo.definitionForName(QStringLiteral("Sieve")));
    syntaxHighlighter->setTheme((palette().color(QPalette::Base).lightness() < 128)
                                ? mSyntaxRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                                : mSyntaxRepo.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
    topLayout->addWidget(mEditor);
    topLayout->addWidget(buttonBox);

    readConfig();
}

FilterConvertToSieveResultDialog::~FilterConvertToSieveResultDialog()
{
    writeConfig();
#ifdef KF5_USE_PURPOSE
    delete mTemporaryShareFile;
#endif
}

void FilterConvertToSieveResultDialog::slotInitializeShareMenu()
{
#ifdef KF5_USE_PURPOSE
    delete mTemporaryShareFile;
    mTemporaryShareFile = new QTemporaryFile();
    mTemporaryShareFile->open();
    mTemporaryShareFile->setPermissions(QFile::ReadUser);
    mTemporaryShareFile->write(mEditor->editor()->toPlainText().toUtf8());
    mTemporaryShareFile->close();
    mShareMenu->model()->setInputData(QJsonObject {
        { QStringLiteral("urls"), QJsonArray { {QUrl::fromLocalFile(mTemporaryShareFile->fileName()).toString()} } },
        { QStringLiteral("mimeType"), { QStringLiteral("text/plain") } }
    });
    mShareMenu->reload();
#endif
}

void FilterConvertToSieveResultDialog::slotShareActionFinished(const QJsonObject &output, int error, const QString &message)
{
#ifdef KF5_USE_PURPOSE
    if (error) {
        KMessageBox::error(this, i18n("There was a problem sharing the document: %1", message),
                           i18n("Share"));
    } else {
        const QString url = output[QLatin1String("url")].toString();
        if (url.isEmpty()) {
            KMessageBox::information(this, i18n("File was shared."));
        } else {
            KMessageBox::information(this, i18n("<qt>You can find the new request at:<br /><a href='%1'>%1</a> </qt>", url),
                    QString(), QString(), KMessageBox::AllowLink);
        }
    }
#endif
}

void FilterConvertToSieveResultDialog::slotSave()
{
    const QString filter = i18n("Sieve Files (*.siv);;All Files (*)");
    PimCommon::Util::saveTextAs(mEditor->editor()->toPlainText(), filter, this, QUrl(),
                                i18nc("@title:window", "Convert to Script Sieve"));
}

void FilterConvertToSieveResultDialog::setCode(const QString &code)
{
    mEditor->editor()->setPlainText(code);
}

static const char myConfigGroupName[] = "FilterConvertToSieveResultDialog";

void FilterConvertToSieveResultDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void FilterConvertToSieveResultDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
    group.sync();
}

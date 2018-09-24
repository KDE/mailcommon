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

#ifndef FILTERCONVERTTOSIEVERESULTDIALOG_H
#define FILTERCONVERTTOSIEVERESULTDIALOG_H

#include <KSyntaxHighlighting/Repository>
#include <QDialog>
class QTemporaryFile;
namespace KPIMTextEdit {
class PlainTextEditorWidget;
}
#ifdef KF5_USE_PURPOSE
namespace Purpose {
class Menu;
}
#endif

namespace MailCommon {
class FilterConvertToSieveResultDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterConvertToSieveResultDialog(QWidget *parent = nullptr);
    ~FilterConvertToSieveResultDialog();

    void setCode(const QString &code);

private Q_SLOTS:
    void slotSave();

private:
    void readConfig();
    void writeConfig();
    void slotShareActionFinished(const QJsonObject &output, int error, const QString &message);
    void slotInitializeShareMenu();
    KPIMTextEdit::PlainTextEditorWidget *mEditor = nullptr;
    KSyntaxHighlighting::Repository mSyntaxRepo;
#ifdef KF5_USE_PURPOSE
    Purpose::Menu *mShareMenu = nullptr;
    QTemporaryFile *mTemporaryShareFile = nullptr;
#endif
};
}

#endif // FILTERCONVERTTOSIEVERESULTDIALOG_H

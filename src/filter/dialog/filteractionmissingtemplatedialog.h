/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef FILTERACTIONMISSINGTEMPLATEDIALOG_H
#define FILTERACTIONMISSINGTEMPLATEDIALOG_H

#include <QDialog>

class QComboBox;
namespace MailCommon
{
class FilterActionMissingTemplateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FilterActionMissingTemplateDialog(const QStringList &templateList, const QString &filtername, QWidget *parent = nullptr);
    ~FilterActionMissingTemplateDialog();
    Q_REQUIRED_RESULT QString selectedTemplate() const;

private:
    void readConfig();
    void writeConfig();
    QComboBox *mComboBoxTemplate = nullptr;
};
}
#endif // FILTERACTIONMISSINGTEMPLATEDIALOG_H

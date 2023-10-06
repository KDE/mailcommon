/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDialog>
class QUrl;

namespace MailCommon
{
class SelectThunderbirdFilterFilesWidget;
class SelectThunderbirdFilterFilesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SelectThunderbirdFilterFilesDialog(const QString &defaultSettingPath, QWidget *parent);
    ~SelectThunderbirdFilterFilesDialog() override;

    [[nodiscard]] QStringList selectedFiles() const;

    void setStartDir(const QUrl &);

private:
    void readConfig();
    void writeConfig();
    SelectThunderbirdFilterFilesWidget *const mSelectFilterFilesWidget;
};
}

/*
  SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class QUrl;
class QAbstractButton;

namespace Ui
{
class SelectThunderbirdFilterFilesWidget;
}
namespace MailCommon
{
class SelectThunderbirdFilterFilesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SelectThunderbirdFilterFilesWidget(const QString &defaultSettingPath, QWidget *parent = nullptr);
    ~SelectThunderbirdFilterFilesWidget() override;
    [[nodiscard]] QStringList selectedFiles() const;

    void setStartDir(const QUrl &);

Q_SIGNALS:
    void enableOkButton(bool);

private:
    void slotButtonClicked(QAbstractButton *button);
    void slotProfileChanged(int);

    void slotUrlChanged(const QString &path);
    void slotItemSelectionChanged();
    Ui::SelectThunderbirdFilterFilesWidget *const ui;
};
}

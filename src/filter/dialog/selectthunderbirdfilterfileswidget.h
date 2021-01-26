/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SELECTTHUNDERBIRDFILTERFILESWIDGET_H
#define SELECTTHUNDERBIRDFILTERFILESWIDGET_H

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
    ~SelectThunderbirdFilterFilesWidget();
    Q_REQUIRED_RESULT QStringList selectedFiles() const;

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
#endif // SELECTTHUNDERBIRDFILTERFILESWIDGET_H

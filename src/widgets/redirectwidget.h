/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#ifndef REDIRECTWIDGET_H
#define REDIRECTWIDGET_H

#include <QWidget>

namespace MessageComposer
{
class ComposerLineEdit;
}

namespace MailCommon
{
class RedirectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RedirectWidget(QWidget *parent = nullptr);
    ~RedirectWidget();

    void setFocus();
    Q_REQUIRED_RESULT QString resend();

Q_SIGNALS:
    void addressChanged(const QString &);

private:
    void slotAddressSelection();
    MessageComposer::ComposerLineEdit *mEdit = nullptr;
    QString mResendStr;
};
}

#endif // REDIRECTWIDGET_H

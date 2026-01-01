/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once

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
    ~RedirectWidget() override;

    void forceFocus();
    [[nodiscard]] QString resend();

Q_SIGNALS:
    void addressChanged(const QString &);

private:
    void slotAddressSelection();
    MessageComposer::ComposerLineEdit *const mEdit;
    QString mResendStr;
};
}

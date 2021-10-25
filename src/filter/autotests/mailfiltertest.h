/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class MailFilterTest : public QObject
{
    Q_OBJECT
public:
    explicit MailFilterTest(QObject *parent = nullptr);
    ~MailFilterTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldApplySettings();
};


/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef MAILFILTERTEST_H
#define MAILFILTERTEST_H

#include <QObject>

class MailFilterTest : public QObject
{
    Q_OBJECT
public:
    explicit MailFilterTest(QObject *parent = nullptr);
    ~MailFilterTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldApplySettings();
};

#endif // MAILFILTERTEST_H

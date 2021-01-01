/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERACTIONREPLYTOTEST_H
#define FILTERACTIONREPLYTOTEST_H

#include <QObject>

class FilterActionReplyToTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionReplyToTest(QObject *parent = nullptr);
    ~FilterActionReplyToTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldBeEmpty();
    void shouldHadReplyToHeader();
    void shouldReplaceReplyToHeader();
    void shouldHaveRequiredPart();
    void shouldNotCreateReplyToWhenAddressIsEmpty();
};

#endif // FILTERACTIONREPLYTOTEST_H

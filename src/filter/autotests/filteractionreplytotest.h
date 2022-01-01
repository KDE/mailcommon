/*
  SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterActionReplyToTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionReplyToTest(QObject *parent = nullptr);
    ~FilterActionReplyToTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldBeEmpty();
    void shouldHadReplyToHeader();
    void shouldReplaceReplyToHeader();
    void shouldHaveRequiredPart();
    void shouldNotCreateReplyToWhenAddressIsEmpty();
};


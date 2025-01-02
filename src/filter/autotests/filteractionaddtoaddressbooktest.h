/*
  SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class FilterActionAddToAddressBookTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionAddToAddressBookTest(QObject *parent = nullptr);
    ~FilterActionAddToAddressBookTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldReportErrorWhenArgumentIsEmpty();
    void shouldReportErrorWhenCollectionIsInvalid();
    void shouldRequiresPart();
};

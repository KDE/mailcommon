/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef FILTERACTIONADDTOADDRESSBOOKTEST_H
#define FILTERACTIONADDTOADDRESSBOOKTEST_H

#include <QObject>

class FilterActionAddToAddressBookTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionAddToAddressBookTest(QObject *parent = nullptr);
    ~FilterActionAddToAddressBookTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldReportErrorWhenArgumentIsEmpty();
    void shouldReportErrorWhenCollectionIsInvalid();
    void shouldRequiresPart();
};

#endif // FILTERACTIONADDTOADDRESSBOOKTEST_H

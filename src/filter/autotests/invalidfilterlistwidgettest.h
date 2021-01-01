/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef INVALIDFILTERLISTWIDGETTEST_H
#define INVALIDFILTERLISTWIDGETTEST_H

#include <QObject>

class InvalidFilterListWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit InvalidFilterListWidgetTest(QObject *parent = nullptr);
    ~InvalidFilterListWidgetTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAddInvalidFilters();
};

#endif // INVALIDFILTERLISTWIDGETTEST_H

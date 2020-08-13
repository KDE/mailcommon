/*
  SPDX-FileCopyrightText: 2015 Tomas Trnka <tomastrnka@gmx.com>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef SEARCHPATTERNTEST_H
#define SEARCHPATTERNTEST_H

#include <QObject>

class SearchPatternTest : public QObject
{
    Q_OBJECT
public:
    explicit SearchPatternTest(QObject *parent = nullptr);
    ~SearchPatternTest();
private Q_SLOTS:
    void shouldRuleRequirePart_data();
    void shouldRuleRequirePart();
};

#endif // SEARCHPATTERNTEST_H

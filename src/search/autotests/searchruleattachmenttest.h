/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QObject>

class SearchRuleAttachmentTest : public QObject
{
    Q_OBJECT
public:
    explicit SearchRuleAttachmentTest(QObject *parent = nullptr);
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldHaveRequirePart();
    void shouldBeEmpty();
};

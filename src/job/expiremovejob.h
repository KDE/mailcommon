/**
 * SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QObject>

class ExpireMoveJob : public QObject
{
    Q_OBJECT
public:
    explicit ExpireMoveJob(QObject *parent = nullptr);
    ~ExpireMoveJob() override;
};

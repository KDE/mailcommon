/*
  SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <Akonadi/Collection>
#include <QHash>
#include <QObject>
namespace MailCommon
{
/*!
 * \brief The FilterImporterPathCache class
 * \author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT FilterImporterPathCache : public QObject
{
    Q_OBJECT
public:
    static FilterImporterPathCache *self();

    explicit FilterImporterPathCache(QObject *parent = nullptr);
    ~FilterImporterPathCache() override;

    void insert(const QString &original, const Akonadi::Collection &newValue);
    [[nodiscard]] Akonadi::Collection convertedFilterPath(const QString &original);
    void clear();
    [[nodiscard]] int count() const;

private:
    QHash<QString, Akonadi::Collection> mFilterCache;
};
}

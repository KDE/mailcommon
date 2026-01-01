/**
 * SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <Akonadi/Item>
#include <QObject>

class ExpireDeleteJob : public QObject
{
    Q_OBJECT
public:
    explicit ExpireDeleteJob(QObject *parent = nullptr);
    ~ExpireDeleteJob() override;

    [[nodiscard]] Akonadi::Item::List removedMsgs() const;
    void setRemovedMsgs(const Akonadi::Item::List &newRemovedMsgs);

    void start();

    [[nodiscard]] QString sourceFolderName() const;
    void setSourceFolderName(const QString &newSourceFolderName);

Q_SIGNALS:
    void expireDeleteDone();

private:
    void finished();
    void slotExpireDone(KJob *job);
    QString mSourceFolderName;
    Akonadi::Item::List mRemovedMsgs;
    QList<KJob *> mRunningJobs;
};

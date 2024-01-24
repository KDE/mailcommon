/**
 * SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <Akonadi/Item>
#include <QObject>

class ExpireMoveJob : public QObject
{
    Q_OBJECT
public:
    explicit ExpireMoveJob(QObject *parent = nullptr);
    ~ExpireMoveJob() override;

    [[nodiscard]] Akonadi::Item::List removedMsgs() const;
    void setRemovedMsgs(const Akonadi::Item::List &newRemovedMsgs);

    void start();

    [[nodiscard]] Akonadi::Collection moveToFolder() const;
    void setMoveToFolder(const Akonadi::Collection &newMoveToFolder);

    [[nodiscard]] QString srcFolderName() const;
    void setSrcFolderName(const QString &newSrcFolderName);

Q_SIGNALS:
    void expireMovedDone();

private:
    void slotMoveDone(KJob *job);
    void slotExpireDone(KJob *job);
    void finished();
    QString mSrcFolderName;
    Akonadi::Collection mMoveToFolder;
    Akonadi::Item::List mRemovedMsgs;
    QList<KJob *> mRunningJobs;
};

/**
 * SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "expiremovejob.h"
#include <Akonadi/ItemMoveJob>

ExpireMoveJob::ExpireMoveJob(QObject *parent)
    : QObject{parent}
{
}

ExpireMoveJob::~ExpireMoveJob() = default;

Akonadi::Item::List ExpireMoveJob::removedMsgs() const
{
    return mRemovedMsgs;
}

void ExpireMoveJob::setRemovedMsgs(const Akonadi::Item::List &newRemovedMsgs)
{
    mRemovedMsgs = newRemovedMsgs;
}

void ExpireMoveJob::start()
{
    // TODO
}

#include "moc_expiremovejob.cpp"

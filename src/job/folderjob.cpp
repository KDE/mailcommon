/*
 *
 *  SPDX-FileCopyrightText: 2003 Zack Rusin <zack@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "folderjob.h"

#include <KIO/Global>

using namespace MailCommon;

//----------------------------------------------------------------------------
FolderJob::FolderJob(QObject *parent)
    : QObject(parent)
{
}

//----------------------------------------------------------------------------
FolderJob::~FolderJob()
{
    Q_EMIT result(this);
    Q_EMIT finished();
}

//----------------------------------------------------------------------------
void FolderJob::start()
{
    if (!mStarted) {
        mStarted = true;
        execute();
    }
}

//----------------------------------------------------------------------------
void FolderJob::kill()
{
    mErrorCode = KIO::ERR_USER_CANCELED;
    delete this;
}

int FolderJob::error() const
{
    return mErrorCode;
}

bool FolderJob::isCancellable() const
{
    return mCancellable;
}

void FolderJob::setCancellable(bool b)
{
    mCancellable = b;
}

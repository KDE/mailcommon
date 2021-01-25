/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractioncopy.h"

#include "kernel/mailkernel.h"
#include "util/mailutil.h"

#include <ItemCopyJob>
#include <KLocalizedString>

using namespace MailCommon;

FilterActionCopy::FilterActionCopy(QObject *parent)
    : FilterActionWithFolder(QStringLiteral("copy"), i18n("Copy Into Folder"), parent)
{
}

FilterAction::ReturnCode FilterActionCopy::process(ItemContext &context, bool) const
{
    // copy the message 1:1
    auto job = new Akonadi::ItemCopyJob(context.item(), mFolder, nullptr);
    connect(job, &Akonadi::ItemCopyJob::result, this, &FilterActionCopy::jobFinished);

    return GoOn;
}

void FilterActionCopy::jobFinished(KJob *job)
{
    if (job->error()) {
        qCritical() << "Error while moving mail: " << job->errorString();
    }
}

SearchRule::RequiredPart FilterActionCopy::requiredPart() const
{
    return SearchRule::Envelope;
}

FilterAction *FilterActionCopy::newAction()
{
    return new FilterActionCopy;
}

QString FilterActionCopy::sieveCode() const
{
    QString path;
    if (KernelIf->collectionModel()) {
        path = MailCommon::Util::fullCollectionPath(mFolder);
    } else {
        path = QString::number(mFolder.id());
    }
    const QString result = QStringLiteral("fileinto :copy \"%1\";").arg(path);
    return result;
}

QStringList FilterActionCopy::sieveRequires() const
{
    return QStringList() << QStringLiteral("fileinto") << QStringLiteral("copy");
}

QString FilterActionCopy::informationAboutNotValidAction() const
{
    return i18n("Folder destination was not defined.");
}

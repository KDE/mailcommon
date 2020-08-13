/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionmove.h"
#include "MailCommon/MailUtil"

#include "kernel/mailkernel.h"

#include <KLocalizedString>

using namespace MailCommon;

FilterAction *FilterActionMove::newAction()
{
    return new FilterActionMove;
}

FilterActionMove::FilterActionMove(QObject *parent)
    : FilterActionWithFolder(QStringLiteral("transfer"), i18n("Move Into Folder"), parent)
{
}

FilterAction::ReturnCode FilterActionMove::process(ItemContext &context, bool) const
{
    if (!mFolder.isValid()) {
        const Akonadi::Collection targetFolder = CommonKernel->collectionFromId(mFolder.id());
        if (!targetFolder.isValid()) {
            return ErrorButGoOn;
        }

        context.setMoveTargetCollection(targetFolder);
    } else {
        context.setMoveTargetCollection(mFolder);
    }
    return GoOn;
}

SearchRule::RequiredPart FilterActionMove::requiredPart() const
{
    return SearchRule::Envelope;
}

QString FilterActionMove::sieveCode() const
{
    QString path;
    if (KernelIf->collectionModel()) {
        path = MailCommon::Util::fullCollectionPath(mFolder, false);
    } else {
        path = QString::number(mFolder.id());
    }
    const QString result = QStringLiteral("fileinto \"%1\";").arg(path);
    return result;
}

QStringList FilterActionMove::sieveRequires() const
{
    return QStringList() << QStringLiteral("fileinto");
}

QString FilterActionMove::informationAboutNotValidAction() const
{
    return i18n("Folder destination was not defined.");
}

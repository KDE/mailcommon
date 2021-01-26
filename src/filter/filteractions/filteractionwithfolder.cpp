/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionwithfolder.h"

#include "filter/dialog/filteractionmissingfolderdialog.h"
#include "filter/filterimporterpathcache.h"
#include "folder/folderrequester.h"
#include "kernel/mailkernel.h"
#include "util/mailutil.h"

#include <QPointer>

using namespace MailCommon;

FilterActionWithFolder::FilterActionWithFolder(const QString &name, const QString &label, QObject *parent)
    : FilterAction(name, label, parent)
{
}

bool FilterActionWithFolder::isEmpty() const
{
    return !mFolder.isValid();
}

QWidget *FilterActionWithFolder::createParamWidget(QWidget *parent) const
{
    auto requester = new FolderRequester(parent);
    requester->setShowOutbox(false);
    setParamWidgetValue(requester);
    requester->setObjectName(QStringLiteral("folderrequester"));

    connect(requester, &FolderRequester::folderChanged, this, &FilterActionWithFolder::filterActionModified);

    return requester;
}

void FilterActionWithFolder::applyParamWidgetValue(QWidget *paramWidget)
{
    mFolder = static_cast<FolderRequester *>(paramWidget)->collection();
}

void FilterActionWithFolder::setParamWidgetValue(QWidget *paramWidget) const
{
    static_cast<FolderRequester *>(paramWidget)->setCollection(mFolder);
}

void FilterActionWithFolder::clearParamWidget(QWidget *paramWidget) const
{
    static_cast<FolderRequester *>(paramWidget)->setCollection(CommonKernel->draftsCollectionFolder());
}

bool FilterActionWithFolder::argsFromStringInteractive(const QString &argsStr, const QString &name)
{
    bool needUpdate = false;
    argsFromString(argsStr);
    if (!mFolder.isValid()) {
        bool exactPath = false;
        const Akonadi::Collection::List lst = FilterActionMissingFolderDialog::potentialCorrectFolders(argsStr, exactPath);
        if (lst.count() == 1 && exactPath) {
            mFolder = lst.at(0);
        } else {
            const Akonadi::Collection newCol = MailCommon::FilterImporterPathCache::self()->convertedFilterPath(argsStr);
            if (!newCol.isValid()) {
                QPointer<FilterActionMissingFolderDialog> dlg = new FilterActionMissingFolderDialog(lst, name, argsStr);
                if (dlg->exec()) {
                    mFolder = dlg->selectedCollection();
                    needUpdate = true;
                    MailCommon::FilterImporterPathCache::self()->insert(argsStr, mFolder);
                }
                delete dlg;
            } else {
                mFolder = newCol;
                needUpdate = true;
            }
        }
    }
    return needUpdate;
}

QString FilterActionWithFolder::argsAsStringReal() const
{
    if (KernelIf->collectionModel()) {
        return MailCommon::Util::fullCollectionPath(mFolder);
    }
    return FilterActionWithFolder::argsAsString();
}

void FilterActionWithFolder::argsFromString(const QString &argsStr)
{
    bool ok = false;
    const Akonadi::Collection::Id id = argsStr.toLongLong(&ok);
    if (ok) {
        mFolder = Akonadi::Collection(id);
    } else {
        mFolder = Akonadi::Collection();
    }
}

QString FilterActionWithFolder::argsAsString() const
{
    QString result;
    if (mFolder.isValid()) {
        result = QString::number(mFolder.id());
    }

    return result;
}

QString FilterActionWithFolder::displayString() const
{
    QString result;
    if (mFolder.isValid()) {
        result = MailCommon::Util::fullCollectionPath(CommonKernel->collectionFromId(mFolder.id()));
    }

    return label() + QLatin1String(" \"") + result.toHtmlEscaped() + QLatin1String("\"");
}

bool FilterActionWithFolder::folderRemoved(const Akonadi::Collection &oldFolder, const Akonadi::Collection &newFolder)
{
    if (oldFolder == mFolder) {
        mFolder = newFolder;
        return true;
    } else {
        return false;
    }
}

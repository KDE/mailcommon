/*
  Copyright (c) 2009-2017 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef MAILCOMMON_FOLDERSETTINGS_H
#define MAILCOMMON_FOLDERSETTINGS_H

#include "mailcommon_export.h"
#include <MessageViewer/Viewer>

#include <MessageCore/MailingList>
using MessageCore::MailingList;

#include <Collection>
#include <CollectionStatistics>

#include <KSharedConfig>
#include <QKeySequence>
#include <KIO/Job>

namespace MailCommon
{

class MAILCOMMON_EXPORT FolderSettings : public QObject
{
    Q_OBJECT

public:
    static QSharedPointer<FolderSettings> forCollection(
        const Akonadi::Collection &coll, bool writeConfig = true);

    ~FolderSettings();

    void setCollection(const Akonadi::Collection &collection);

    static QString configGroupName(const Akonadi::Collection &col);
    static void clearCache();
    static void resetHtmlFormat();

    bool isWriteConfig() const;
    void setWriteConfig(bool writeConfig);

    void writeConfig() const;
    void readConfig();

    QString name() const;

    bool isReadOnly() const;

    bool isStructural() const;

    bool isSystemFolder() const;

    qint64 count() const;

    bool canDeleteMessages() const;

    bool canCreateMessages() const;

    bool isValid() const;

    Akonadi::Collection::Rights rights() const;

    Akonadi::CollectionStatistics statistics() const;

    void setShortcut(const QKeySequence &);
    const QKeySequence &shortcut() const;

    /**
     *  Get / set whether the default identity should be used instead of the
     *  identity specified by setIdentity().
     */
    void setUseDefaultIdentity(bool useDefaultIdentity);
    bool useDefaultIdentity() const;

    void setIdentity(uint identity);
    uint identity() const;

    /**
     * Returns true if this folder is associated with a mailing-list.
     */
    void setMailingListEnabled(bool enabled);
    bool isMailingListEnabled() const;

    void setMailingList(const MailingList &mlist);

    MailingList mailingList() const;

    /**
     * Returns true if the replies to mails from this folder should be
     * put in the same folder.
     */
    bool putRepliesInSameFolder() const;
    void setPutRepliesInSameFolder(bool b);

    /**
     * Returns true if this folder should be hidden from all folder selection dialogs
     */
    bool hideInSelectionDialog() const;
    void setHideInSelectionDialog(bool hide);

    QString mailingListPostAddress() const;

    uint fallBackIdentity() const;
    MessageViewer::Viewer::DisplayFormatMessage formatMessage() const;
    void setFormatMessage(MessageViewer::Viewer::DisplayFormatMessage formatMessage);

protected Q_SLOTS:
    void slotIdentitiesChanged();

private:
    explicit FolderSettings(const Akonadi::Collection &col, bool writeconfig);

    Akonadi::Collection mCollection;

    /** Mailing list attributes */
    bool                mMailingListEnabled;
    MailingList         mMailingList;

    bool mUseDefaultIdentity;
    uint mIdentity;

    MessageViewer::Viewer::DisplayFormatMessage mFormatMessage;
    /** Should replies to messages in this folder be put in here? */
    bool mPutRepliesInSameFolder;

    /** Should this folder be hidden in the folder selection dialog? */
    bool mHideInSelectionDialog;

    /** shortcut associated with this folder or null, if none is configured. */
    QKeySequence mShortcut;
    bool mWriteConfig;
};

}

#endif
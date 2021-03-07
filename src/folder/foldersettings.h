/*
  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_FOLDERSETTINGS_H
#define MAILCOMMON_FOLDERSETTINGS_H

#include "mailcommon_export.h"
#include <MessageViewer/Viewer>

#include <MessageCore/MailingList>
using MessageCore::MailingList;

#include <Collection>
#include <CollectionStatistics>

#include <KIO/Job>
#include <KSharedConfig>
#include <QKeySequence>

namespace MailCommon
{
/**
 * @brief The FolderSettings class
 * @author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT FolderSettings : public QObject
{
    Q_OBJECT

public:
    static QSharedPointer<FolderSettings> forCollection(const Akonadi::Collection &coll, bool writeConfig = true);

    ~FolderSettings() override;

    void setCollection(const Akonadi::Collection &collection);

    static QString configGroupName(const Akonadi::Collection &col);
    static void clearCache();
    static void resetHtmlFormat();

    Q_REQUIRED_RESULT bool isWriteConfig() const;
    void setWriteConfig(bool writeConfig);

    void writeConfig() const;
    void readConfig();

    Q_REQUIRED_RESULT QString name() const;

    Q_REQUIRED_RESULT bool isReadOnly() const;

    Q_REQUIRED_RESULT bool isStructural() const;

    Q_REQUIRED_RESULT bool isSystemFolder() const;

    Q_REQUIRED_RESULT qint64 count() const;

    Q_REQUIRED_RESULT bool canDeleteMessages() const;

    Q_REQUIRED_RESULT bool canCreateMessages() const;

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT Akonadi::Collection::Rights rights() const;

    Q_REQUIRED_RESULT Akonadi::CollectionStatistics statistics() const;

    void setShortcut(const QKeySequence &);
    const QKeySequence &shortcut() const;

    /**
     *  Get / set whether the default identity should be used instead of the
     *  identity specified by setIdentity().
     */
    void setUseDefaultIdentity(bool useDefaultIdentity);
    Q_REQUIRED_RESULT bool useDefaultIdentity() const;

    void setIdentity(uint identity);
    Q_REQUIRED_RESULT uint identity() const;

    /**
     * Returns true if this folder is associated with a mailing-list.
     */
    void setMailingListEnabled(bool enabled);
    Q_REQUIRED_RESULT bool isMailingListEnabled() const;

    void setMailingList(const MailingList &mlist);

    MailingList mailingList() const;

    /**
     * Returns true if the replies to mails from this folder should be
     * put in the same folder.
     */
    Q_REQUIRED_RESULT bool putRepliesInSameFolder() const;
    void setPutRepliesInSameFolder(bool b);

    /**
     * Returns true if this folder should be hidden from all folder selection dialogs
     */
    Q_REQUIRED_RESULT bool hideInSelectionDialog() const;
    void setHideInSelectionDialog(bool hide);

    Q_REQUIRED_RESULT QString mailingListPostAddress() const;

    Q_REQUIRED_RESULT uint fallBackIdentity() const;
    Q_REQUIRED_RESULT MessageViewer::Viewer::DisplayFormatMessage formatMessage() const;
    void setFormatMessage(MessageViewer::Viewer::DisplayFormatMessage formatMessage);

    Q_REQUIRED_RESULT bool folderHtmlLoadExtPreference() const;
    void setFolderHtmlLoadExtPreference(bool folderHtmlLoadExtPreference);

protected Q_SLOTS:
    void slotIdentitiesChanged();

private:
    explicit FolderSettings(const Akonadi::Collection &col, bool writeconfig);
    QString resource() const;

    Akonadi::Collection mCollection;

    /** Mailing list attributes */
    bool mMailingListEnabled;
    MailingList mMailingList;

    bool mUseDefaultIdentity;
    uint mIdentity;

    MessageViewer::Viewer::DisplayFormatMessage mFormatMessage = MessageViewer::Viewer::Unknown;
    /** Should replies to messages in this folder be put in here? */
    bool mPutRepliesInSameFolder = false;

    /** Should this folder be hidden in the folder selection dialog? */
    bool mHideInSelectionDialog = false;

    bool mFolderHtmlLoadExtPreference = false;

    /** shortcut associated with this folder or null, if none is configured. */
    QKeySequence mShortcut;
    bool mWriteConfig;
};
}

#endif

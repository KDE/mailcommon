/*
  SPDX-FileCopyrightText: 2009-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <MessageViewer/Viewer>

#include <MessageCore/MailingList>
using MessageCore::MailingList;

#include <Akonadi/Collection>
#include <Akonadi/CollectionStatistics>

#include <KSharedConfig>
#include <QKeySequence>
#include <QSharedPointer>

namespace MailCommon
{
/*!
 * \class MailCommon::FolderSettings
 * \inmodule MailCommon
 * \inheaderfile MailCommon/FolderSettings
 *
 * \brief The FolderSettings class.
 *
 * \author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT FolderSettings : public QObject
{
    Q_OBJECT

public:
    /*!
     * Returns the settings of the collection \a coll, creating and caching them if needed.
     * If \a writeConfig is \c true, the settings are written to the configuration when destroyed.
     */
    static QSharedPointer<FolderSettings> forCollection(const Akonadi::Collection &coll, bool writeConfig = true);

    /*!
     * Destroys the folder settings, writing them to the configuration if needed.
     */
    ~FolderSettings() override;

    /*!
     * Sets the \a collection these settings apply to.
     */
    void setCollection(const Akonadi::Collection &collection);

    /*!
     * Returns the name of the configuration group of the collection \a col.
     */
    static QString configGroupName(const Akonadi::Collection &col);
    /*!
     * Clears the cache of folder settings.
     */
    static void clearCache();
    /*!
     * Resets the message display format of all the cached folder settings to the global setting.
     */
    static void resetHtmlFormat();

    /*!
     * Returns \c true if the settings are written to the configuration when destroyed.
     */
    [[nodiscard]] bool isWriteConfig() const;
    /*!
     * Sets whether the settings are written to the configuration when destroyed, according to \a writeConfig.
     */
    void setWriteConfig(bool writeConfig);

    /*!
     * Writes the settings to the configuration.
     */
    void writeConfig() const;
    /*!
     * Reads the settings from the configuration.
     */
    void readConfig();

    /*!
     * Returns the name of the collection.
     */
    [[nodiscard]] QString name() const;

    /*!
     * Returns \c true if the collection is read-only.
     */
    [[nodiscard]] bool isReadOnly() const;

    /*!
     * Returns \c true if the collection is structural, i.e. it cannot contain messages.
     */
    [[nodiscard]] bool isStructural() const;

    /*!
     * Returns \c true if the collection is a system folder.
     */
    [[nodiscard]] bool isSystemFolder() const;

    /*!
     * Returns the number of messages in the collection.
     */
    [[nodiscard]] qint64 count() const;
    /*!
     * Returns \c true if messages can be deleted from the collection.
     */
    [[nodiscard]] bool canDeleteMessages() const;

    /*!
     * Returns \c true if messages can be created in the collection.
     */
    [[nodiscard]] bool canCreateMessages() const;

    /*!
     * Returns \c true if the collection is valid.
     */
    [[nodiscard]] bool isValid() const;

    /*!
     * Returns the access rights of the collection.
     */
    [[nodiscard]] Akonadi::Collection::Rights rights() const;

    /*!
     * Returns the statistics of the collection.
     */
    [[nodiscard]] Akonadi::CollectionStatistics statistics() const;

    /*!
     * Sets the keyboard \a shortcut used to select the folder.
     */
    void setShortcut(const QKeySequence &shortcut);
    /*!
     * Returns the keyboard shortcut used to select the folder.
     */
    const QKeySequence &shortcut() const;

    /*!
     *  Sets whether the default identity should be used instead of the
     *  identity specified by setIdentity(), according to \a useDefaultIdentity.
     */
    void setUseDefaultIdentity(bool useDefaultIdentity);
    /*!
     * Returns \c true if the default identity is used instead of the identity specified by setIdentity().
     */
    [[nodiscard]] bool useDefaultIdentity() const;

    /*!
     * Sets the \a identity used for this folder.
     */
    void setIdentity(uint identity);
    /*!
     * Returns the identity used for this folder.
     */
    [[nodiscard]] uint identity() const;

    /*!
     * Sets whether this folder is associated with a mailing-list, according to \a enabled.
     */
    void setMailingListEnabled(bool enabled);
    /*!
     * Returns \c true if this folder is associated with a mailing-list.
     */
    [[nodiscard]] bool isMailingListEnabled() const;

    /*!
     * Sets the mailing-list \a mlist associated with this folder.
     */
    void setMailingList(const MailingList &mlist);

    /*!
     * Returns the mailing-list associated with this folder.
     */
    [[nodiscard]] MailingList mailingList() const;

    /*!
     * Returns true if the replies to mails from this folder should be
     * put in the same folder.
     */
    [[nodiscard]] bool putRepliesInSameFolder() const;
    void setPutRepliesInSameFolder(bool b);

    /*!
     * Returns true if this folder should be hidden from all folder selection dialogs
     */
    [[nodiscard]] bool hideInSelectionDialog() const;
    /*!
     * Sets whether this folder is hidden from all folder selection dialogs, according to \a hide.
     */
    void setHideInSelectionDialog(bool hide);

    /*!
     * Returns the post address of the mailing-list associated with this folder.
     */
    [[nodiscard]] QString mailingListPostAddress() const;

    /*!
     * Returns the identity to use for this folder, taking the account identity into account.
     */
    [[nodiscard]] uint fallBackIdentity() const;
    /*!
     * Returns the format used to display the messages of this folder.
     */
    [[nodiscard]] MessageViewer::Viewer::DisplayFormatMessage formatMessage() const;
    /*!
     * Sets the format used to display the messages of this folder to \a formatMessage.
     */
    void setFormatMessage(MessageViewer::Viewer::DisplayFormatMessage formatMessage);

    /*!
     * Returns \c true if external references are loaded in HTML messages of this folder.
     */
    [[nodiscard]] bool folderHtmlLoadExtPreference() const;
    /*!
     * Sets whether external references are loaded in HTML messages of this folder, according to \a folderHtmlLoadExtPreference.
     */
    void setFolderHtmlLoadExtPreference(bool folderHtmlLoadExtPreference);

protected Q_SLOTS:
    /*!
     * Updates the identity of this folder when the identities change.
     */
    void slotIdentitiesChanged();

private:
    explicit MAILCOMMON_NO_EXPORT FolderSettings(const Akonadi::Collection &col, bool writeconfig);
    [[nodiscard]] MAILCOMMON_NO_EXPORT QString resource() const;

    Akonadi::Collection mCollection;

    /*! Mailing list attributes */
    bool mMailingListEnabled = false;
    MailingList mMailingList;

    bool mUseDefaultIdentity = true;
    uint mIdentity;

    MessageViewer::Viewer::DisplayFormatMessage mFormatMessage = MessageViewer::Viewer::Unknown;
    /*! Should replies to messages in this folder be put in here? */
    bool mPutRepliesInSameFolder = false;

    /*! Should this folder be hidden in the folder selection dialog? */
    bool mHideInSelectionDialog = false;

    bool mFolderHtmlLoadExtPreference = false;

    /*! shortcut associated with this folder or null, if none is configured. */
    QKeySequence mShortcut;
    bool mWriteConfig = true;
};
}

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

    [[nodiscard]] bool isWriteConfig() const;
    void setWriteConfig(bool writeConfig);

    void writeConfig() const;
    void readConfig();

    [[nodiscard]] QString name() const;

    [[nodiscard]] bool isReadOnly() const;

    [[nodiscard]] bool isStructural() const;

    [[nodiscard]] bool isSystemFolder() const;

    [[nodiscard]] qint64 count() const;

    [[nodiscard]] bool canDeleteMessages() const;

    [[nodiscard]] bool canCreateMessages() const;

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] Akonadi::Collection::Rights rights() const;

    [[nodiscard]] Akonadi::CollectionStatistics statistics() const;

    void setShortcut(const QKeySequence &);
    const QKeySequence &shortcut() const;

    /**
     *  Get / set whether the default identity should be used instead of the
     *  identity specified by setIdentity().
     */
    void setUseDefaultIdentity(bool useDefaultIdentity);
    [[nodiscard]] bool useDefaultIdentity() const;

    void setIdentity(uint identity);
    [[nodiscard]] uint identity() const;

    /**
     * Returns true if this folder is associated with a mailing-list.
     */
    void setMailingListEnabled(bool enabled);
    [[nodiscard]] bool isMailingListEnabled() const;

    void setMailingList(const MailingList &mlist);

    [[nodiscard]] MailingList mailingList() const;

    /**
     * Returns true if the replies to mails from this folder should be
     * put in the same folder.
     */
    [[nodiscard]] bool putRepliesInSameFolder() const;
    void setPutRepliesInSameFolder(bool b);

    /**
     * Returns true if this folder should be hidden from all folder selection dialogs
     */
    [[nodiscard]] bool hideInSelectionDialog() const;
    void setHideInSelectionDialog(bool hide);

    [[nodiscard]] QString mailingListPostAddress() const;

    [[nodiscard]] uint fallBackIdentity() const;
    [[nodiscard]] MessageViewer::Viewer::DisplayFormatMessage formatMessage() const;
    void setFormatMessage(MessageViewer::Viewer::DisplayFormatMessage formatMessage);

    [[nodiscard]] bool folderHtmlLoadExtPreference() const;
    void setFolderHtmlLoadExtPreference(bool folderHtmlLoadExtPreference);

protected Q_SLOTS:
    void slotIdentitiesChanged();

private:
    explicit MAILCOMMON_NO_EXPORT FolderSettings(const Akonadi::Collection &col, bool writeconfig);
    [[nodiscard]] MAILCOMMON_NO_EXPORT QString resource() const;

    Akonadi::Collection mCollection;

    /** Mailing list attributes */
    bool mMailingListEnabled = false;
    MailingList mMailingList;

    bool mUseDefaultIdentity = true;
    uint mIdentity;

    MessageViewer::Viewer::DisplayFormatMessage mFormatMessage = MessageViewer::Viewer::Unknown;
    /** Should replies to messages in this folder be put in here? */
    bool mPutRepliesInSameFolder = false;

    /** Should this folder be hidden in the folder selection dialog? */
    bool mHideInSelectionDialog = false;

    bool mFolderHtmlLoadExtPreference = false;

    /** shortcut associated with this folder or null, if none is configured. */
    QKeySequence mShortcut;
    bool mWriteConfig = true;
};
}

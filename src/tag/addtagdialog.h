/*
  SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include "tag.h"
#include <Akonadi/Tag>
#include <QDialog>
#include <QList>
#include <memory>

class KActionCollection;
class KJob;

namespace MailCommon
{
class AddTagDialogPrivate;
/*!
 * \class MailCommon::AddTagDialog
 * \inmodule MailCommon
 * \inheaderfile MailCommon/AddTagDialog
 *
 * \brief The AddTagDialog class.
 */
class MAILCOMMON_EXPORT AddTagDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * Constructs an add tag dialog.
     *
     * \a actions List of action collections for keyboard shortcuts
     * \a parent The parent widget
     */
    explicit AddTagDialog(const QList<KActionCollection *> &actions, QWidget *parent = nullptr);
    /*!
     * Destroys the add tag dialog.
     */
    ~AddTagDialog() override;

    /*!
     * Sets the list of existing tags to validate against duplicates.
     *
     * \a tags The list of existing tags
     */
    void setTags(const QList<MailCommon::Tag::Ptr> &tags);
    /*!
     * Returns the label (name) of the tag being created.
     *
     * \return The tag label
     */
    [[nodiscard]] QString label() const;
    /*!
     * Returns the Akonadi tag that was created.
     *
     * \return The created Akonadi tag
     */
    [[nodiscard]] Akonadi::Tag tag() const;

private:
    MAILCOMMON_NO_EXPORT void slotSave();
    MAILCOMMON_NO_EXPORT void slotTagNameChanged(const QString &text);
    MAILCOMMON_NO_EXPORT void onTagCreated(KJob *job);
    std::unique_ptr<AddTagDialogPrivate> const d;
};
}

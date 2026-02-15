/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "mailcommon_export.h"
#include <QAbstractItemModel>
#include <QWidget>
class KActionCollection;
class SnippetWidgetPrivate;
namespace MailCommon
{
/*!
 * \class MailCommon::SnippetWidget
 * \inmodule MailCommon
 * \inheaderfile MailCommon/SnippetWidget
 *
 * \brief The SnippetWidget class
 * \author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT SnippetWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructs a snippet widget.
     *
     * \param parent The parent widget
     */
    explicit SnippetWidget(QWidget *parent = nullptr);
    /*!
     * Destroys the snippet widget.
     */
    ~SnippetWidget() override;

    /*!
     * Sets the name of the snippet.
     *
     * \param name The snippet name
     */
    void setName(const QString &name);
    /*!
     * Returns the name of the snippet.
     *
     * \return The snippet name
     */
    [[nodiscard]] QString name() const;

    /*!
     * Sets the text content of the snippet.
     *
     * \param text The snippet text
     */
    void setText(const QString &text);
    /*!
     * Returns the text content of the snippet.
     *
     * \return The snippet text
     */
    [[nodiscard]] QString text() const;

    /*!
     * Sets the keyboard shortcut for this snippet.
     *
     * \param sequence The key sequence
     */
    void setKeySequence(const QKeySequence &sequence);
    /*!
     * Returns the keyboard shortcut for this snippet.
     *
     * \return The key sequence
     */
    [[nodiscard]] QKeySequence keySequence() const;

    /*!
     * Sets the keyword that triggers this snippet.
     *
     * \param keyword The trigger keyword
     */
    void setKeyword(const QString &keyword);
    /*!
     * Returns the keyword that triggers this snippet.
     *
     * \return The trigger keyword
     */
    [[nodiscard]] QString keyword() const;

    /*!
     * Sets the "To" email address for this snippet.
     *
     * \param keyword The email address
     */
    void setTo(const QString &keyword);
    /*!
     * Returns the "To" email address for this snippet.
     *
     * \return The email address
     */
    [[nodiscard]] QString to() const;

    /*!
     * Sets the "CC" email addresses for this snippet.
     *
     * \param keyword The email addresses
     */
    void setCc(const QString &keyword);
    /*!
     * Returns the "CC" email addresses for this snippet.
     *
     * \return The email addresses
     */
    [[nodiscard]] QString cc() const;

    /*!
     * Sets the "BCC" email addresses for this snippet.
     *
     * \param keyword The email addresses
     */
    void setBcc(const QString &keyword);
    /*!
     * Returns the "BCC" email addresses for this snippet.
     *
     * \return The email addresses
     */
    [[nodiscard]] QString bcc() const;

    /*!
     * Sets the group model for organizing snippets.
     *
     * \param model The abstract item model
     */
    void setGroupModel(QAbstractItemModel *model);
    /*!
     * Sets the current group index.
     *
     * \param index The model index of the group
     */
    void setGroupIndex(const QModelIndex &index);

    /*!
     * Returns the current group index.
     *
     * \return The model index of the selected group
     */
    [[nodiscard]] QModelIndex groupIndex() const;
    /*!
     * Checks if the snippet is valid for saving.
     *
     * \return True if the snippet is valid, false otherwise
     */
    [[nodiscard]] bool snippetIsValid() const;

    /*!
     * Sets the action collections for checking keyboard shortcut conflicts.
     *
     * \param lst The list of action collections
     */
    void setCheckActionCollections(const QList<KActionCollection *> &lst);

    /*!
     * Sets whether the snippet group is selected.
     *
     * \param b True if the group is selected
     */
    void setGroupSelected(bool b);

    /*!
     * Returns whether a snippet group is selected.
     *
     * \return True if a group is selected, false otherwise
     */
    [[nodiscard]] bool isGroupSelected() const;
    /*!
     * Clears all fields in the widget.
     */
    void clear();

    /*!
     * Returns whether the snippet was modified.
     *
     * \return True if the snippet was changed
     */
    [[nodiscard]] bool wasChanged() const;
    /*!
     * Sets the modified state of the snippet.
     *
     * \param b True if the snippet was changed
     */
    void setWasChanged(bool b);

    /*!
     * Sets the subject line for this snippet.
     *
     * \param text The subject text
     */
    void setSubject(const QString &text);
    /*!
     * Returns the subject line for this snippet.
     *
     * \return The subject text
     */
    [[nodiscard]] QString subject() const;

    /*!
     * Sets the attachment path for this snippet.
     *
     * \param keyword The attachment path
     */
    void setAttachment(const QString &keyword);
    /*!
     * Returns the attachment path for this snippet.
     *
     * \return The attachment path
     */
    [[nodiscard]] QString attachment() const;

Q_SIGNALS:
    /*!
     * Emitted when the text content of the snippet has changed.
     *
     * \param str The new text
     */
    void textChanged(const QString &str);
    /*!
     * Emitted when the group selection has changed.
     *
     * \param index The new group index
     */
    void groupChanged(int index);

private:
    std::unique_ptr<SnippetWidgetPrivate> const d;
};
}

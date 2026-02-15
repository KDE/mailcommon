/*
  SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"

#include "tag.h"

#include <QWidget>

class QLineEdit;
class KColorCombo;
class KIconButton;
class KKeySequenceWidget;
class QCheckBox;
class KActionCollection;

namespace MailCommon
{
class TagWidgetPrivate;
/*!
 * \class MailCommon::TagWidget
 * \inmodule MailCommon
 * \inheaderfile MailCommon/TagWidget
 *
 * \brief The TagWidget class
 */
class MAILCOMMON_EXPORT TagWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructs a tag widget for editing tag properties.
     *
     * \param actionCollections List of action collections for keyboard shortcuts
     * \param parent The parent widget
     */
    explicit TagWidget(const QList<KActionCollection *> &actionCollections, QWidget *parent = nullptr);
    /*!
     * Destroys the tag widget.
     */
    ~TagWidget() override;

    /*!
     * Records the current tag settings to the given tag object.
     *
     * \param tag The tag object to store settings in
     */
    void recordTagSettings(MailCommon::Tag::Ptr tag);

    /*!
     * Returns the line edit widget for the tag name.
     *
     * \return A pointer to the tag name line edit widget
     */
    [[nodiscard]] QLineEdit *tagNameLineEdit() const;
    /*!
     * Returns the checkbox for enabling text color selection.
     *
     * \return A pointer to the text color checkbox
     */
    [[nodiscard]] QCheckBox *textColorCheck() const;
    /*!
     * Returns the checkbox for enabling text font formatting.
     *
     * \return A pointer to the text font checkbox
     */
    [[nodiscard]] QCheckBox *textFontCheck() const;
    /*!
     * Returns the checkbox for enabling background color selection.
     *
     * \return A pointer to the background color checkbox
     */
    [[nodiscard]] QCheckBox *backgroundColorCheck() const;
    /*!
     * Returns the checkbox for showing the tag in the toolbar.
     *
     * \return A pointer to the toolbar checkbox
     */
    [[nodiscard]] QCheckBox *inToolBarCheck() const;

    /*!
     * Returns the color combo box for selecting text color.
     *
     * \return A pointer to the text color combo box
     */
    [[nodiscard]] KColorCombo *textColorCombo() const;
    /*!
     * Returns the color combo box for selecting background color.
     *
     * \return A pointer to the background color combo box
     */
    [[nodiscard]] KColorCombo *backgroundColorCombo() const;

    /*!
     * Returns the checkbox for bold text formatting.
     *
     * \return A pointer to the text bold checkbox
     */
    [[nodiscard]] QCheckBox *textBoldCheck() const;
    /*!
     * Returns the checkbox for italic text formatting.
     *
     * \return A pointer to the text italic checkbox
     */
    [[nodiscard]] QCheckBox *textItalicCheck() const;

    /*!
     * Returns the button widget for selecting the tag icon.
     *
     * \return A pointer to the icon button widget
     */
    [[nodiscard]] KIconButton *iconButton() const;

    /*!
     * Returns the widget for selecting keyboard shortcut.
     *
     * \return A pointer to the key sequence widget
     */
    [[nodiscard]] KKeySequenceWidget *keySequenceWidget() const;

    /*!
     * Sets the text color for the tag preview.
     *
     * \param color The color to set
     */
    void setTagTextColor(const QColor &color);
    /*!
     * Sets the background color for the tag preview.
     *
     * \param color The color to set
     */
    void setTagBackgroundColor(const QColor &color);
    /*!
     * Sets the text format (bold and italic) for the tag preview.
     *
     * \param bold Whether the text should be bold
     * \param italic Whether the text should be italic
     */
    void setTagTextFormat(bool bold, bool italic);

Q_SIGNALS:
    /*!
     * Emitted when any tag setting has been changed.
     */
    void changed();
    /*!
     * Emitted when the tag's icon name has been changed.
     *
     * \param iconName The new icon name
     */
    void iconNameChanged(const QString &iconName);

private:
    MAILCOMMON_NO_EXPORT void slotEmitChangeCheck();
    std::unique_ptr<TagWidgetPrivate> const d;
};
}

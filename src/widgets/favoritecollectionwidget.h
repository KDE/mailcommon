/*

  SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"

#include <Akonadi/EntityListView>

class KXMLGUIClient;
class KActionCollection;

namespace MailCommon
{
class MailCommonSettings;
/*!
 * \class MailCommon::FavoriteCollectionWidget
 * \inmodule MailCommon
 * \inheaderfile MailCommon/FavoriteCollectionWidget
 *
 * \brief The FavoriteCollectionWidget class.
 *
 * \author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT FavoriteCollectionWidget : public Akonadi::EntityListView
{
    Q_OBJECT
public:
    /*!
     * Constructs a favorite collection widget.
     *
     * \a settings The mail common settings object
     * \a xmlGuiClient The XML GUI client for action management
     * \a parent The parent widget
     */
    explicit FavoriteCollectionWidget(MailCommon::MailCommonSettings *settings, KXMLGUIClient *xmlGuiClient, QWidget *parent = nullptr);
    /*!
     * Destroys the favorite collection widget.
     */
    ~FavoriteCollectionWidget() override;

    /*!
     * Reads the configuration for the widget.
     */
    void readConfig();
    /*!
     * Updates the display mode of the widget.
     */
    void updateMode();

    /*!
     * Changes the view mode of the favorite collection list.
     *
     * \a mode The new view mode (Icon, List, etc.)
     */
    void changeViewMode(QListView::ViewMode mode);

protected Q_SLOTS:
    /*!
     * Slot called when the general font in KDE settings has changed.
     */
    void slotGeneralFontChanged();
    /*!
     * Slot called when the general palette in KDE settings has changed.
     */
    void slotGeneralPaletteChanged();
    /*!
     * Slot called when the icon size should be changed.
     *
     * \a toggle Whether to toggle the icon size
     */
    void slotChangeIconSize(bool toggle);
    /*!
     * Slot called when the display mode should be changed.
     *
     * \a toggle Whether to toggle the mode
     */
    void slotChangeMode(bool toggle);

protected:
    /*!
     * Handles paint events to draw the favorite collections.
     *
     * \a event The paint event
     */
    void paintEvent(QPaintEvent *event) override;
    /*!
     * Handles drag enter events for drag and drop operations.
     *
     * \a event The drag enter event
     */
    void dragEnterEvent(QDragEnterEvent *event) override;
    /*!
     * Handles drag move events for drag and drop operations.
     *
     * \a event The drag move event
     */
    void dragMoveEvent(QDragMoveEvent *event) override;
    /*!
     * Handles drop events for drag and drop operations.
     *
     * \a event The drop event
     */
    void dropEvent(QDropEvent *event) override;
    /*!
     * Initiates drag operations.
     *
     * \a supportedActions The drop actions supported by the source
     */
    void startDrag(Qt::DropActions supportedActions) override;

    /*!
     * Handles mouse press events.
     *
     * \a e The mouse event
     */
    void mousePressEvent(QMouseEvent *e) override;
    /*!
     * Processes events with special handling.
     *
     * \a e The event to process
     * \return True if the event was processed, false otherwise
     */
    [[nodiscard]] bool event(QEvent *e) override;
Q_SIGNALS:
    /*!
     * Emitted when a new tab is requested.
     *
     * \a isChecked True if the action that triggered this signal is checked
     */
    void newTabRequested(bool isChecked);

private:
    [[nodiscard]] MAILCOMMON_NO_EXPORT bool acceptEvent(QDropEvent *event) const;
    MAILCOMMON_NO_EXPORT void createMenu(KActionCollection *ac);
    MAILCOMMON_NO_EXPORT void updatePalette();

    class FavoriteCollectionWidgetPrivate;
    std::unique_ptr<FavoriteCollectionWidgetPrivate> const d;
};
}

/*

  SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>

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
/**
 * @brief The FavoriteCollectionWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT FavoriteCollectionWidget : public Akonadi::EntityListView
{
    Q_OBJECT
public:
    explicit FavoriteCollectionWidget(MailCommon::MailCommonSettings *settings, KXMLGUIClient *xmlGuiClient, QWidget *parent = nullptr);
    ~FavoriteCollectionWidget() override;

    void readConfig();
    void updateMode();

    void changeViewMode(QListView::ViewMode mode);

protected Q_SLOTS:
    void slotGeneralFontChanged();
    void slotGeneralPaletteChanged();
    void slotChangeIconSize(bool);
    void slotChangeMode(bool);

protected:
    void paintEvent(QPaintEvent *) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void startDrag(Qt::DropActions) override;

    void mousePressEvent(QMouseEvent *e) override;
Q_SIGNALS:
    void newTabRequested(bool);

private:
    bool acceptEvent(QDropEvent *event) const;
    void createMenu(KActionCollection *ac);

    class FavoriteCollectionWidgetPrivate;
    std::unique_ptr<FavoriteCollectionWidgetPrivate> const d;
};
}

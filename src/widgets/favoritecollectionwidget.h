/*

  Copyright (c) 2012-2019 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef MAILCOMMON_FAVORITECOLLECTIONWIDGET_H
#define MAILCOMMON_FAVORITECOLLECTIONWIDGET_H

#include "mailcommon_export.h"

#include <EntityListView>

class KXMLGUIClient;
class KActionCollection;

namespace MailCommon {
class MailCommonSettings;
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

    class Private;
    Private *const d;
};
}

#endif /* MAILCOMMON_FAVORITECOLLECTIONWIDGET_H */

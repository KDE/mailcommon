/*

  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "favoritecollectionwidget.h"
#include "kernel/mailkernel.h"
#include "mailcommonsettings_base.h"

#include <MessageCore/MessageCoreSettings>

#include <AkonadiWidgets/CollectionStatisticsDelegate>
#include <KActionCollection>
#include <KActionMenu>
#include <KLocalizedString>
#include <KXMLGUIClient>

#include <QFontDatabase>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>

using namespace MailCommon;

class Q_DECL_HIDDEN FavoriteCollectionWidget::Private
{
public:
    Private()
    {
    }

    QColor textColor;
    QAction *listMode = nullptr;
    QAction *iconMode = nullptr;
    MailCommonSettings *settings = nullptr;
};

FavoriteCollectionWidget::FavoriteCollectionWidget(MailCommon::MailCommonSettings *settings, KXMLGUIClient *xmlGuiClient, QWidget *parent)
    : Akonadi::EntityListView(xmlGuiClient, parent)
    , d(new Private)
{
    d->settings = settings;
    setFocusPolicy(Qt::NoFocus);

    auto delegate = new Akonadi::CollectionStatisticsDelegate(this);
    delegate->setProgressAnimationEnabled(true);

    setItemDelegate(delegate);

    delegate->setUnreadCountShown(true);

    readConfig();

    createMenu(xmlGuiClient->actionCollection());
}

FavoriteCollectionWidget::~FavoriteCollectionWidget()
{
    delete d;
}

void FavoriteCollectionWidget::mousePressEvent(QMouseEvent *e)
{
    const bool buttonPressedIsMiddle = (e->button() == Qt::MiddleButton);
    Q_EMIT newTabRequested(buttonPressedIsMiddle);
    Akonadi::EntityListView::mousePressEvent(e);
}

void FavoriteCollectionWidget::updateMode()
{
    switch (viewMode()) {
    case ListMode:
        d->listMode->setChecked(true);
        d->iconMode->setChecked(false);
        break;
    case IconMode:
        d->listMode->setChecked(false);
        d->iconMode->setChecked(true);
        break;
    }
}

void FavoriteCollectionWidget::createMenu(KActionCollection *ac)
{
    auto iconSizeMenu = new KActionMenu(i18n("Icon size"), this);
    ac->addAction(QStringLiteral("favorite_icon_size"), iconSizeMenu);

    static const int icon_sizes[] = {16, 22, 32 /*, 48, 64, 128 */};

    auto grp = new QActionGroup(iconSizeMenu);
    QAction *act = nullptr;
    for (int i : icon_sizes) {
        act = new QAction(QStringLiteral("%1x%2").arg(i).arg(i), iconSizeMenu);
        iconSizeMenu->addAction(act);
        act->setCheckable(true);
        grp->addAction(act);
        if (iconSize().width() == i) {
            act->setChecked(true);
        }
        act->setData(QVariant(i));
        connect(act, &QAction::triggered, this, &FavoriteCollectionWidget::slotChangeIconSize);
    }

    auto modeFavoriteMenu = new KActionMenu(i18n("Mode"), this);
    ac->addAction(QStringLiteral("favorite_mode"), modeFavoriteMenu);

    grp = new QActionGroup(modeFavoriteMenu);
    d->listMode = new QAction(i18n("List Mode"), modeFavoriteMenu);
    modeFavoriteMenu->addAction(d->listMode);
    d->listMode->setCheckable(true);
    grp->addAction(d->listMode);
    if (viewMode() == ListMode) {
        d->listMode->setChecked(true);
    }
    d->listMode->setData(QVariant(MailCommon::MailCommonSettings::EnumFavoriteCollectionViewMode::ListMode));
    connect(d->listMode, &QAction::triggered, this, &FavoriteCollectionWidget::slotChangeMode);

    d->iconMode = new QAction(i18n("Icon Mode"), modeFavoriteMenu);
    modeFavoriteMenu->addAction(d->iconMode);
    grp->addAction(d->iconMode);
    d->iconMode->setCheckable(true);
    if (viewMode() == IconMode) {
        d->iconMode->setChecked(true);
    }
    d->iconMode->setData(QVariant(MailCommon::MailCommonSettings::EnumFavoriteCollectionViewMode::IconMode));
    connect(d->iconMode, &QAction::triggered, this, &FavoriteCollectionWidget::slotChangeMode);
}

void FavoriteCollectionWidget::slotChangeMode(bool)
{
    auto act = qobject_cast<QAction *>(sender());
    if (!act) {
        return;
    }

    QVariant data = act->data();

    bool ok;
    const int mode = data.toInt(&ok);
    if (!ok) {
        return;
    }

    switch (mode) {
    case MailCommon::MailCommonSettings::EnumFavoriteCollectionViewMode::IconMode:
        changeViewMode(IconMode);
        break;
    case MailCommon::MailCommonSettings::EnumFavoriteCollectionViewMode::ListMode:
        changeViewMode(ListMode);
        break;
    }

    d->settings->setFavoriteCollectionViewMode(mode);
    d->settings->save();
}

void FavoriteCollectionWidget::changeViewMode(QListView::ViewMode mode)
{
    setViewMode(mode);
    setDragEnabled(true);
    setAcceptDrops(true);
}

void FavoriteCollectionWidget::slotChangeIconSize(bool)
{
    auto act = qobject_cast<QAction *>(sender());
    if (!act) {
        return;
    }

    QVariant data = act->data();

    bool ok;
    const int size = data.toInt(&ok);
    if (!ok) {
        return;
    }

    const QSize newIconSize(QSize(size, size));
    if (newIconSize == iconSize()) {
        return;
    }
    setIconSize(newIconSize);
    d->settings->setIconSize(iconSize().width());
    d->settings->save();
}

void FavoriteCollectionWidget::slotGeneralPaletteChanged()
{
    const QPalette palette = viewport()->palette();
    QColor color = palette.text().color();
    color.setAlpha(128);
    d->textColor = color;
}

void FavoriteCollectionWidget::slotGeneralFontChanged()
{
    // Custom/System font support
    if (MessageCore::MessageCoreSettings::self()->useDefaultFonts()) {
        setFont(QFontDatabase::systemFont(QFontDatabase::GeneralFont));
    }
}

void FavoriteCollectionWidget::readConfig()
{
    setFont(QFontDatabase::systemFont(QFontDatabase::GeneralFont));

    int iIconSize = d->settings->iconSize();
    if (iIconSize < 16 || iIconSize > 32) {
        iIconSize = 22;
    }
    setIconSize(QSize(iIconSize, iIconSize));
}

void FavoriteCollectionWidget::paintEvent(QPaintEvent *event)
{
    if (!model() || model()->rowCount() == 0) {
        QPainter p(viewport());

        QFont font = p.font();
        font.setItalic(true);
        p.setFont(font);

        if (!d->textColor.isValid()) {
            slotGeneralPaletteChanged();
        }
        p.setPen(d->textColor);

        p.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, i18n("Drop your favorite folders here..."));
    } else {
        Akonadi::EntityListView::paintEvent(event);
    }
}

static bool isCollection(const QMimeData *mimeData)
{
    const QList<QUrl> urls = mimeData->urls();
    for (const QUrl &url : urls) {
        const Akonadi::Collection collection = Akonadi::Collection::fromUrl(url);
        if (collection.isValid()) {
            return true;
        }
    }
    return false;
}

bool FavoriteCollectionWidget::acceptEvent(QDropEvent *event) const
{
    const bool draggingCollection = isCollection(event->mimeData());
    const bool droppingOnCollection = dropIndicatorPosition() == QAbstractItemView::OnItem;
    if (event->source() == this) {
        if (draggingCollection && !droppingOnCollection) { // Re-ordering favorites
            return true;
        }
    } else {
        if ((draggingCollection && !droppingOnCollection) // Adding a new favorite collection
            || (!draggingCollection && droppingOnCollection)) { // Dropping emails onto a favorite collection
            return true;
        }
    }
    event->ignore();
    return false;
}

void FavoriteCollectionWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->source() == this) {
        QListView::dragEnterEvent(event); // Re-ordering favourites
    } else {
        Akonadi::EntityListView::dragEnterEvent(event); // Dropping emails onto a favorite collection
    }
}

void FavoriteCollectionWidget::dragMoveEvent(QDragMoveEvent *event)
{
    // We need to ask QListView to update dropIndicatorPosition() first...
    QListView::dragMoveEvent(event);
    if (event->source() == this) {
        if (acceptEvent(event)) {
            event->setDropAction(Qt::MoveAction);
            event->accept(); // Re-ordering favourites
        }
    } else {
        if (acceptEvent(event)) {
            Akonadi::EntityListView::dragMoveEvent(event); // Dropping emails onto a favorite collection
        }
    }
}

void FavoriteCollectionWidget::dropEvent(QDropEvent *event)
{
    if (event->source() == this) {
        if (acceptEvent(event)) {
            QListView::dropEvent(event); // Re-ordering favourites
        }
    } else {
        if (acceptEvent(event)) {
            if (dropIndicatorPosition() == QAbstractItemView::OnItem) {
                Akonadi::EntityListView::dropEvent(event); // Dropping emails onto a favorite collection
            } else {
                QListView::dropEvent(event); // Add new favorite
            }
        }
    }
}

void FavoriteCollectionWidget::startDrag(Qt::DropActions supportedActions)
{
    // skip EntityListView logic (we want to reorder favorites, not trigger moving/copying of actual folders)
    QListView::startDrag(supportedActions);
}

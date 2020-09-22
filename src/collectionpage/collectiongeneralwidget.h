/*
  SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef COLLECTIONGENERALWIDGET_H
#define COLLECTIONGENERALWIDGET_H

#include <QWidget>
#include "mailcommon_export.h"
#include <AkonadiCore/Collection>
class QCheckBox;
namespace MailCommon {
class MAILCOMMON_EXPORT CollectionGeneralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CollectionGeneralWidget(QWidget *parent = nullptr);
    ~CollectionGeneralWidget();
    void save(Akonadi::Collection &);
    void load(const Akonadi::Collection &col);
private:
    QCheckBox *mNotifyOnNewMailCheckBox = nullptr;
    QCheckBox *mKeepRepliesInSameFolderCheckBox = nullptr;
    QCheckBox *mHideInSelectionDialogCheckBox = nullptr;
    QCheckBox *mUseDefaultIdentityCheckBox = nullptr;

};
}
#endif // COLLECTIONGENERALWIDGET_H

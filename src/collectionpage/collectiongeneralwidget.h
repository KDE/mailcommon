/*
  SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <Akonadi/Collection>
#include <QWidget>
class QCheckBox;
namespace KIdentityManagement
{
class IdentityCombo;
}
namespace MailCommon
{
class FolderSettings;
class MAILCOMMON_EXPORT CollectionGeneralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CollectionGeneralWidget(QWidget *parent = nullptr);
    ~CollectionGeneralWidget() override;
    void save(Akonadi::Collection &);
    void load(const Akonadi::Collection &col);

private:
    void slotIdentityCheckboxChanged();
    QCheckBox *mNotifyOnNewMailCheckBox = nullptr;
    QCheckBox *mKeepRepliesInSameFolderCheckBox = nullptr;
    QCheckBox *mHideInSelectionDialogCheckBox = nullptr;
    QCheckBox *mUseDefaultIdentityCheckBox = nullptr;
    KIdentityManagement::IdentityCombo *mIdentityComboBox = nullptr;
    QSharedPointer<MailCommon::FolderSettings> mFolderCollection;
};
}

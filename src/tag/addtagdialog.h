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

#ifndef ADDTAGDIALOG_H
#define ADDTAGDIALOG_H

#include "mailcommon_export.h"
#include "tag.h"
#include <QVector>
#include <QDialog>
#include <AkonadiCore/Tag>
#include <AkonadiCore/TagCreateJob>

class KActionCollection;

namespace MailCommon {
class AddTagDialogPrivate;
class MAILCOMMON_EXPORT AddTagDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddTagDialog(const QList<KActionCollection *> &actions, QWidget *parent = nullptr);
    ~AddTagDialog();

    void setTags(const QVector<MailCommon::Tag::Ptr> &tags);
    Q_REQUIRED_RESULT QString label() const;
    Q_REQUIRED_RESULT Akonadi::Tag tag() const;

private:
    void slotSave();
    void slotTagNameChanged(const QString &text);
    void onTagCreated(KJob *job);
    AddTagDialogPrivate *const d;
};
}
#endif // ADDTAGDIALOG_H

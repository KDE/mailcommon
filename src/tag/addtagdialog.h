/*
  SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include "tag.h"
#include <Akonadi/Tag>
#include <Akonadi/TagCreateJob>
#include <QDialog>
#include <QList>

class KActionCollection;

namespace MailCommon
{
class AddTagDialogPrivate;
class MAILCOMMON_EXPORT AddTagDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddTagDialog(const QList<KActionCollection *> &actions, QWidget *parent = nullptr);
    ~AddTagDialog() override;

    void setTags(const QList<MailCommon::Tag::Ptr> &tags);
    [[nodiscard]] QString label() const;
    [[nodiscard]] Akonadi::Tag tag() const;

private:
    MAILCOMMON_NO_EXPORT void slotSave();
    MAILCOMMON_NO_EXPORT void slotTagNameChanged(const QString &text);
    MAILCOMMON_NO_EXPORT void onTagCreated(KJob *job);
    std::unique_ptr<AddTagDialogPrivate> const d;
};
}

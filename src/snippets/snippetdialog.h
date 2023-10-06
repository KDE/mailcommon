/*
  snippet feature from kdevelop/plugins/snippet/

  SPDX-FileCopyrightText: 2007 Robert Gruber <rgruber@users.sourceforge.net>
  SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDialog>

class KActionCollection;

class QAbstractItemModel;
class QModelIndex;
class QPushButton;

namespace MailCommon
{
class SnippetWidget;
class SnippetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SnippetDialog(KActionCollection *actionCollection, bool inGroupMode, QWidget *parent = nullptr);
    ~SnippetDialog() override;

    void setName(const QString &name);
    [[nodiscard]] QString name() const;

    void setText(const QString &text);
    [[nodiscard]] QString text() const;

    void setSubject(const QString &text);
    [[nodiscard]] QString subject() const;

    void setKeySequence(const QKeySequence &sequence);
    [[nodiscard]] QKeySequence keySequence() const;

    void setKeyword(const QString &keyword);
    [[nodiscard]] QString keyword() const;

    void setTo(const QString &keyword);
    [[nodiscard]] QString to() const;

    void setCc(const QString &keyword);
    [[nodiscard]] QString cc() const;

    void setBcc(const QString &keyword);
    [[nodiscard]] QString bcc() const;

    void setGroupModel(QAbstractItemModel *model);

    void setGroupIndex(const QModelIndex &index);
    [[nodiscard]] QModelIndex groupIndex() const;

    void setAttachment(const QString &keyword);
    [[nodiscard]] QString attachment() const;

private:
    void slotTextChanged();
    void slotGroupChanged();
    bool snippetIsValid() const;
    void writeConfig();
    void readConfig();

    KActionCollection *mActionCollection = nullptr;
    SnippetWidget *const mSnippetWidget;
    QPushButton *mOkButton = nullptr;
    bool mInGroupMode = false;
};
}

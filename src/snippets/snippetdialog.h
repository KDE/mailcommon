/*
  snippet feature from kdevelop/plugins/snippet/

  SPDX-FileCopyrightText: 2007 Robert Gruber <rgruber@users.sourceforge.net>
  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_SNIPPETDIALOG_P_H
#define MAILCOMMON_SNIPPETDIALOG_P_H

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
    Q_REQUIRED_RESULT QString name() const;

    void setText(const QString &text);
    Q_REQUIRED_RESULT QString text() const;

    void setSubject(const QString &text);
    Q_REQUIRED_RESULT QString subject() const;

    void setKeySequence(const QKeySequence &sequence);
    Q_REQUIRED_RESULT QKeySequence keySequence() const;

    void setKeyword(const QString &keyword);
    Q_REQUIRED_RESULT QString keyword() const;

    void setTo(const QString &keyword);
    Q_REQUIRED_RESULT QString to() const;

    void setCc(const QString &keyword);
    Q_REQUIRED_RESULT QString cc() const;

    void setBcc(const QString &keyword);
    Q_REQUIRED_RESULT QString bcc() const;

    void setGroupModel(QAbstractItemModel *model);

    void setGroupIndex(const QModelIndex &index);
    Q_REQUIRED_RESULT QModelIndex groupIndex() const;

    void setAttachment(const QString &keyword);
    Q_REQUIRED_RESULT QString attachment() const;
private Q_SLOTS:
    void slotTextChanged();
    void slotGroupChanged();

private:
    bool snippetIsValid() const;
    void writeConfig();
    void readConfig();

    KActionCollection *mActionCollection = nullptr;
    SnippetWidget *mSnippetWidget = nullptr;
    QPushButton *mOkButton = nullptr;
    bool mInGroupMode = false;
};
}
#endif

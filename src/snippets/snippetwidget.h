/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef SNIPPETWIDGET_H
#define SNIPPETWIDGET_H

#include "mailcommon_export.h"
#include <QAbstractItemModel>
#include <QWidget>
class KActionCollection;
class SnippetWidgetPrivate;
namespace MailCommon
{
/**
 * @brief The SnippetWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT SnippetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SnippetWidget(QWidget *parent = nullptr);
    ~SnippetWidget() override;

    void setName(const QString &name);
    Q_REQUIRED_RESULT QString name() const;

    void setText(const QString &text);
    Q_REQUIRED_RESULT QString text() const;

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
    Q_REQUIRED_RESULT bool snippetIsValid() const;

    void setCheckActionCollections(const QList<KActionCollection *> &lst);

    void setGroupSelected(bool b);

    Q_REQUIRED_RESULT bool isGroupSelected() const;
    void clear();

    Q_REQUIRED_RESULT bool wasChanged() const;
    void setWasChanged(bool b);

    void setSubject(const QString &text);
    Q_REQUIRED_RESULT QString subject() const;

    void setAttachment(const QString &keyword);
    Q_REQUIRED_RESULT QString attachment() const;

Q_SIGNALS:
    void textChanged(const QString &str);
    void groupChanged(int index);

private:
    SnippetWidgetPrivate *const d;
};
}

#endif // SNIPPETWIDGET_H

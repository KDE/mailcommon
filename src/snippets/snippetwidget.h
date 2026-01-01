/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

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
    [[nodiscard]] QString name() const;

    void setText(const QString &text);
    [[nodiscard]] QString text() const;

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
    [[nodiscard]] bool snippetIsValid() const;

    void setCheckActionCollections(const QList<KActionCollection *> &lst);

    void setGroupSelected(bool b);

    [[nodiscard]] bool isGroupSelected() const;
    void clear();

    [[nodiscard]] bool wasChanged() const;
    void setWasChanged(bool b);

    void setSubject(const QString &text);
    [[nodiscard]] QString subject() const;

    void setAttachment(const QString &keyword);
    [[nodiscard]] QString attachment() const;

Q_SIGNALS:
    void textChanged(const QString &str);
    void groupChanged(int index);

private:
    std::unique_ptr<SnippetWidgetPrivate> const d;
};
}

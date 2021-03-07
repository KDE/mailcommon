/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef COLLECTIONTEMPLATESWIDGET_H
#define COLLECTIONTEMPLATESWIDGET_H

#include "mailcommon_export.h"
#include <AkonadiCore/Collection>
#include <QWidget>
class QCheckBox;
namespace TemplateParser
{
class TemplatesConfiguration;
}

namespace MailCommon
{
/**
 * @brief The CollectionTemplateWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT CollectionTemplatesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CollectionTemplatesWidget(QWidget *parent = nullptr);
    ~CollectionTemplatesWidget() override;
    void save(Akonadi::Collection &);
    void load(const Akonadi::Collection &col);

private:
    void slotCopyGlobal();
    void slotChanged();
    QCheckBox *mCustom = nullptr;
    TemplateParser::TemplatesConfiguration *mWidget = nullptr;
    QString mCollectionId;
    uint mIdentity = 0;
    bool mChanged = false;
};
}

#endif // COLLECTIONTEMPLATESWIDGET_H

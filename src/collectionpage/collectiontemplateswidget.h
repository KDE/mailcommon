/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <Akonadi/Collection>
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
    MAILCOMMON_NO_EXPORT void slotCopyGlobal();
    MAILCOMMON_NO_EXPORT void slotChanged();
    QCheckBox *const mCustom;
    TemplateParser::TemplatesConfiguration *mWidget = nullptr;
    QString mCollectionId;
    uint mIdentity = 0;
    bool mChanged = false;
};
}

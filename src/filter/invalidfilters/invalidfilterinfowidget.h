/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef INVALIDFILTERINFOWIDGET_H
#define INVALIDFILTERINFOWIDGET_H

#include "mailcommon_private_export.h"
#include <KMessageWidget>
namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT InvalidFilterInfoWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit InvalidFilterInfoWidget(QWidget *parent = nullptr);
    ~InvalidFilterInfoWidget();
public Q_SLOTS:
    void slotShowDetails(const QString &details);
};
}

#endif // INVALIDFILTERINFOWIDGET_H

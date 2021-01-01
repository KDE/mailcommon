/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "invalidfilterinfowidget.h"

using namespace MailCommon;

InvalidFilterInfoWidget::InvalidFilterInfoWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Information);
    setWordWrap(true);
}

InvalidFilterInfoWidget::~InvalidFilterInfoWidget()
{
}

void InvalidFilterInfoWidget::slotShowDetails(const QString &details)
{
    setText(details);
    animatedShow();
}

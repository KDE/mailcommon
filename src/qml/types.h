// SPDX-FileCopyrightText: 2026 Carl Schwan <carl@carlschwan.eu>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <MailCommon/EntityCollectionOrderProxyModel>

#include <QObject>
#include <qqmlintegration.h>

class EntityCollectionOrderProxyModelForeign : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(EntityCollectionOrderProxyModel)
    QML_FOREIGN(MailCommon::EntityCollectionOrderProxyModel)
    QML_UNCREATABLE("Created by MailCommon")
};

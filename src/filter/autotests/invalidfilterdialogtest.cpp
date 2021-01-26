/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "invalidfilterdialogtest.h"
#include "../filter/invalidfilters/invalidfilterdialog.h"
#include "../filter/invalidfilters/invalidfilterinfowidget.h"
#include "../filter/invalidfilters/invalidfilterwidget.h"
#include <QStandardPaths>
#include <QTest>

InvalidFilterDialogTest::InvalidFilterDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

InvalidFilterDialogTest::~InvalidFilterDialogTest()
{
}

void InvalidFilterDialogTest::shouldHaveDefaultValue()
{
    MailCommon::InvalidFilterDialog dlg;
    auto *widget = dlg.findChild<MailCommon::InvalidFilterWidget *>(QStringLiteral("invalid_filter_widget"));
    QVERIFY(widget);

    auto *infoWidget = dlg.findChild<MailCommon::InvalidFilterInfoWidget *>(QStringLiteral("invalid_filter_infowidget"));
    QVERIFY(infoWidget);
    QVERIFY(!infoWidget->isVisible());
    QVERIFY(!dlg.windowTitle().isEmpty());
}

QTEST_MAIN(InvalidFilterDialogTest)

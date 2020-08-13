/*
  SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "invalidfilterdialogtest.h"
#include "../filter/invalidfilters/invalidfilterdialog.h"
#include <QTest>
#include "../filter/invalidfilters/invalidfilterwidget.h"
#include "../filter/invalidfilters/invalidfilterinfowidget.h"
#include <QStandardPaths>

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
    MailCommon::InvalidFilterWidget *widget = dlg.findChild<MailCommon::InvalidFilterWidget *>(QStringLiteral("invalid_filter_widget"));
    QVERIFY(widget);

    MailCommon::InvalidFilterInfoWidget *infoWidget = dlg.findChild<MailCommon::InvalidFilterInfoWidget *>(QStringLiteral("invalid_filter_infowidget"));
    QVERIFY(infoWidget);
    QVERIFY(!infoWidget->isVisible());
    QVERIFY(!dlg.windowTitle().isEmpty());
}

QTEST_MAIN(InvalidFilterDialogTest)

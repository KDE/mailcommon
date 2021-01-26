/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filterconverttosieveresultdialogtest.h"
#include "../filterconverter/filterconverttosieveresultdialog.h"
#include <KPIMTextEdit/PlainTextEditorWidget>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QTest>
FilterConvertToSieveResultDialogTest::FilterConvertToSieveResultDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

FilterConvertToSieveResultDialogTest::~FilterConvertToSieveResultDialogTest()
{
}

void FilterConvertToSieveResultDialogTest::shouldHaveDefaultValue()
{
    MailCommon::FilterConvertToSieveResultDialog dlg;
    auto *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    auto *saveButton = dlg.findChild<QPushButton *>(QStringLiteral("savebutton"));
    QVERIFY(saveButton);

    auto *editor = dlg.findChild<KPIMTextEdit::PlainTextEditorWidget *>(QStringLiteral("editor"));
    QVERIFY(editor);
    QVERIFY(editor->toPlainText().isEmpty());
}

void FilterConvertToSieveResultDialogTest::shouldAddCode()
{
    MailCommon::FilterConvertToSieveResultDialog dlg;

    auto *editor = dlg.findChild<KPIMTextEdit::PlainTextEditorWidget *>(QStringLiteral("editor"));
    QVERIFY(editor->toPlainText().isEmpty());
    const QString code = QStringLiteral("foo");
    dlg.setCode(code);
    QCOMPARE(editor->toPlainText(), code);
}

QTEST_MAIN(FilterConvertToSieveResultDialogTest)

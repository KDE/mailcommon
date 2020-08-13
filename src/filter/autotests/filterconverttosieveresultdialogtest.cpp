/*
  SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filterconverttosieveresultdialogtest.h"
#include "../filterconverter/filterconverttosieveresultdialog.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QPushButton>
#include <KPIMTextEdit/PlainTextEditorWidget>
#include <QStandardPaths>
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
    QDialogButtonBox *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    QPushButton *saveButton = dlg.findChild<QPushButton *>(QStringLiteral("savebutton"));
    QVERIFY(saveButton);

    KPIMTextEdit::PlainTextEditorWidget *editor = dlg.findChild<KPIMTextEdit::PlainTextEditorWidget *>(QStringLiteral("editor"));
    QVERIFY(editor);
    QVERIFY(editor->toPlainText().isEmpty());
}

void FilterConvertToSieveResultDialogTest::shouldAddCode()
{
    MailCommon::FilterConvertToSieveResultDialog dlg;

    KPIMTextEdit::PlainTextEditorWidget *editor = dlg.findChild<KPIMTextEdit::PlainTextEditorWidget *>(QStringLiteral("editor"));
    QVERIFY(editor->toPlainText().isEmpty());
    const QString code = QStringLiteral("foo");
    dlg.setCode(code);
    QCOMPARE(editor->toPlainText(), code);
}

QTEST_MAIN(FilterConvertToSieveResultDialogTest)

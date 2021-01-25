/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionwithtest.h"

#include "filter/soundtestwidget.h"

using namespace MailCommon;

FilterActionWithTest::FilterActionWithTest(const QString &name, const QString &label, QObject *parent)
    : FilterAction(name, label, parent)
{
}

FilterActionWithTest::~FilterActionWithTest()
{
}

bool FilterActionWithTest::isEmpty() const
{
    return mParameter.trimmed().isEmpty();
}

QWidget *FilterActionWithTest::createParamWidget(QWidget *parent) const
{
    auto soundWidget = new SoundTestWidget(parent);
    soundWidget->setUrl(mParameter);
    soundWidget->setObjectName(QStringLiteral("soundwidget"));

    connect(soundWidget, &SoundTestWidget::textChanged, this, &FilterActionWithTest::filterActionModified);

    return soundWidget;
}

void FilterActionWithTest::applyParamWidgetValue(QWidget *paramWidget)
{
    mParameter = static_cast<SoundTestWidget *>(paramWidget)->url();
}

void FilterActionWithTest::setParamWidgetValue(QWidget *paramWidget) const
{
    static_cast<SoundTestWidget *>(paramWidget)->setUrl(mParameter);
}

void FilterActionWithTest::clearParamWidget(QWidget *paramWidget) const
{
    static_cast<SoundTestWidget *>(paramWidget)->clear();
}

void FilterActionWithTest::argsFromString(const QString &argsStr)
{
    mParameter = argsStr;
}

QString FilterActionWithTest::argsAsString() const
{
    return mParameter;
}

QString FilterActionWithTest::displayString() const
{
    return label() + QLatin1String(" \"") + argsAsString().toHtmlEscaped() + QLatin1String("\"");
}

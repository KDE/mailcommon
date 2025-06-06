/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionwithurl.h"

#include <KLocalizedString>
#include <KUrlRequester>
#include <QUrl>

#include <QHBoxLayout>
#include <QWhatsThis>

using namespace MailCommon;

FilterActionWithUrlHelpButton::FilterActionWithUrlHelpButton(QWidget *parent)
    : QToolButton(parent)
{
    setToolTip(i18nc("@info:tooltip", "Help"));
    setIcon(QIcon::fromTheme(QStringLiteral("help-hint")));
}

FilterActionWithUrlHelpButton::~FilterActionWithUrlHelpButton() = default;

FilterActionWithUrl::FilterActionWithUrl(const QString &name, const QString &label, QObject *parent)
    : FilterAction(name, label, parent)

{
}

FilterActionWithUrl::~FilterActionWithUrl() = default;

bool FilterActionWithUrl::isEmpty() const
{
    return mParameter.trimmed().isEmpty();
}

QWidget *FilterActionWithUrl::createParamWidget(QWidget *parent) const
{
    auto widget = new QWidget(parent);
    auto layout = new QHBoxLayout;
    layout->setContentsMargins({});
    layout->setSpacing(0);
    widget->setLayout(layout);
    auto requester = new KUrlRequester(parent);
    requester->setUrl(QUrl::fromLocalFile(mParameter));
    requester->setObjectName(QLatin1StringView("requester"));
    layout->addWidget(requester);
    mHelpButton = new FilterActionWithUrlHelpButton(parent);
    mHelpButton->setObjectName(QLatin1StringView("helpbutton"));
    connect(mHelpButton, &QAbstractButton::clicked, this, &FilterActionWithUrl::slotHelp);
    layout->addWidget(mHelpButton);

    connect(requester, &KUrlRequester::textChanged, this, &FilterActionWithUrl::filterActionModified);

    return widget;
}

void FilterActionWithUrl::slotHelp()
{
    const QString fullWhatsThis = i18n("You can get specific header when you use %{headername}.");
    QWhatsThis::showText(QCursor::pos(), fullWhatsThis, mHelpButton);
}

void FilterActionWithUrl::applyParamWidgetValue(QWidget *paramWidget)
{
    auto requester = paramWidget->findChild<KUrlRequester *>(QStringLiteral("requester"));
    Q_ASSERT(requester);

    mParameter = requester->text();
}

void FilterActionWithUrl::setParamWidgetValue(QWidget *paramWidget) const
{
    auto requester = paramWidget->findChild<KUrlRequester *>(QStringLiteral("requester"));
    Q_ASSERT(requester);

    requester->setText(mParameter);
}

void FilterActionWithUrl::clearParamWidget(QWidget *paramWidget) const
{
    auto requester = paramWidget->findChild<KUrlRequester *>(QStringLiteral("requester"));
    Q_ASSERT(requester);
    requester->clear();
}

void FilterActionWithUrl::argsFromString(const QString &argsStr)
{
    mParameter = argsStr;
}

QString FilterActionWithUrl::argsAsString() const
{
    return mParameter;
}

QString FilterActionWithUrl::displayString() const
{
    return label() + QLatin1StringView(" \"") + argsAsString().toHtmlEscaped() + QLatin1StringView("\"");
}

#include "moc_filteractionwithurl.cpp"

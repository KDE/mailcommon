/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionrewriteheader.h"

#include <KComboBox>
#include <KLineEdit>
#include <KLocalizedString>

#include <QHBoxLayout>
#include <QLabel>

using namespace MailCommon;

FilterAction *FilterActionRewriteHeader::newAction()
{
    return new FilterActionRewriteHeader;
}

FilterActionRewriteHeader::FilterActionRewriteHeader(QObject *parent)
    : FilterActionWithStringList(QStringLiteral("rewrite header"), i18n("Rewrite Header"), parent)
{
    mParameterList << QString() << QStringLiteral("Subject") << QStringLiteral("Reply-To") << QStringLiteral("Delivered-To")
                   << QStringLiteral("X-KDE-PR-Message") << QStringLiteral("X-KDE-PR-Package") << QStringLiteral("X-KDE-PR-Keywords");

    mParameter = mParameterList.at(0);
}

bool FilterActionRewriteHeader::isEmpty() const
{
    return mParameter.isEmpty() || mRegex.pattern().isEmpty();
}

QString FilterActionRewriteHeader::informationAboutNotValidAction() const
{
    QString info;
    if (mParameter.isEmpty()) {
        info = i18n("Header not defined");
    }
    if (mRegex.pattern().isEmpty()) {
        if (!info.isEmpty()) {
            info += QLatin1Char('\n');
        }
        info += i18n("Search string is empty.");
    }
    return info;
}

FilterAction::ReturnCode FilterActionRewriteHeader::process(ItemContext &context, bool) const
{
    if (isEmpty()) {
        return ErrorButGoOn;
    }

    const auto msg = context.item().payload<KMime::Message::Ptr>();

    const QByteArray param(mParameter.toLatin1());
    KMime::Headers::Base *header = msg->headerByType(param.constData());
    if (!header) {
        return GoOn; // TODO: Maybe create a new header by type?
    }

    QString value = header->asUnicodeString();
    const QString oldValue = value;
    const QString newValue = value.replace(mRegex, mReplacementString);
    if (newValue != oldValue) {
        msg->removeHeader(param.constData());

        KMime::Headers::Base *newheader = KMime::Headers::createHeader(param);
        if (!newheader) {
            newheader = new KMime::Headers::Generic(param.constData());
        }
        newheader->fromUnicodeString(newValue, "utf-8");
        msg->setHeader(newheader);
        msg->assemble();

        context.setNeedsPayloadStore();
    }
    return GoOn;
}

SearchRule::RequiredPart FilterActionRewriteHeader::requiredPart() const
{
    return SearchRule::CompleteMessage;
}

QWidget *FilterActionRewriteHeader::createParamWidget(QWidget *parent) const
{
    auto widget = new QWidget(parent);
    auto layout = new QHBoxLayout(widget);
    layout->setSpacing(4);
    layout->setContentsMargins(0, 0, 0, 0);

    auto comboBox = new KComboBox(widget);
    comboBox->setMinimumWidth(50);
    comboBox->setEditable(true);
    comboBox->setObjectName(QStringLiteral("combo"));
    comboBox->setInsertPolicy(QComboBox::InsertAtBottom);
    layout->addWidget(comboBox, 0 /* stretch */);

    KCompletion *comp = comboBox->completionObject();
    comp->setIgnoreCase(true);
    comp->insertItems(mParameterList);
    comp->setCompletionMode(KCompletion::CompletionPopupAuto);

    auto label = new QLabel(i18n("Replace:"), widget);
    label->setObjectName(QStringLiteral("label_replace"));
    label->setFixedWidth(label->sizeHint().width());
    layout->addWidget(label, 0);

    auto regExpLineEdit = new KLineEdit(widget);
    regExpLineEdit->setClearButtonEnabled(true);
    regExpLineEdit->setTrapReturnKey(true);
    regExpLineEdit->setObjectName(QStringLiteral("search"));
    layout->addWidget(regExpLineEdit, 1);

    label = new QLabel(i18n("With:"), widget);
    label->setFixedWidth(label->sizeHint().width());
    label->setObjectName(QStringLiteral("label_with"));
    layout->addWidget(label, 0);

    auto lineEdit = new KLineEdit(widget);
    lineEdit->setObjectName(QStringLiteral("replace"));
    lineEdit->setClearButtonEnabled(true);
    lineEdit->setTrapReturnKey(true);
    layout->addWidget(lineEdit, 1);

    setParamWidgetValue(widget);
    connect(comboBox, QOverload<int>::of(&KComboBox::currentIndexChanged), this, &FilterActionRewriteHeader::filterActionModified);
    connect(comboBox->lineEdit(), &QLineEdit::textChanged, this, &FilterAction::filterActionModified);
    connect(regExpLineEdit, &KLineEdit::textChanged, this, &FilterActionRewriteHeader::filterActionModified);
    connect(lineEdit, &KLineEdit::textChanged, this, &FilterActionRewriteHeader::filterActionModified);

    return widget;
}

void FilterActionRewriteHeader::setParamWidgetValue(QWidget *paramWidget) const
{
    const int index = mParameterList.indexOf(mParameter);
    const auto comboBox = paramWidget->findChild<KComboBox *>(QStringLiteral("combo"));
    Q_ASSERT(comboBox);

    comboBox->clear();
    comboBox->addItems(mParameterList);
    if (index < 0) {
        comboBox->addItem(mParameter);
        comboBox->setCurrentIndex(comboBox->count() - 1);
    } else {
        comboBox->setCurrentIndex(index);
    }

    auto *regExpLineEdit = paramWidget->findChild<KLineEdit *>(QStringLiteral("search"));
    Q_ASSERT(regExpLineEdit);
    regExpLineEdit->setText(mRegex.pattern());

    auto *lineEdit = paramWidget->findChild<KLineEdit *>(QStringLiteral("replace"));
    Q_ASSERT(lineEdit);
    lineEdit->setText(mReplacementString);
}

void FilterActionRewriteHeader::applyParamWidgetValue(QWidget *paramWidget)
{
    const auto comboBox = paramWidget->findChild<KComboBox *>(QStringLiteral("combo"));
    Q_ASSERT(comboBox);
    mParameter = comboBox->currentText();

    const KLineEdit *regExpLineEdit = paramWidget->findChild<KLineEdit *>(QStringLiteral("search"));
    Q_ASSERT(regExpLineEdit);
    mRegex.setPattern(regExpLineEdit->text());

    const KLineEdit *lineEdit = paramWidget->findChild<KLineEdit *>(QStringLiteral("replace"));
    Q_ASSERT(lineEdit);
    mReplacementString = lineEdit->text();
}

void FilterActionRewriteHeader::clearParamWidget(QWidget *paramWidget) const
{
    const auto comboBox = paramWidget->findChild<KComboBox *>(QStringLiteral("combo"));
    Q_ASSERT(comboBox);
    comboBox->setCurrentIndex(0);

    auto *regExpLineEdit = paramWidget->findChild<KLineEdit *>(QStringLiteral("search"));
    Q_ASSERT(regExpLineEdit);
    regExpLineEdit->clear();

    auto *lineEdit = paramWidget->findChild<KLineEdit *>(QStringLiteral("replace"));
    Q_ASSERT(lineEdit);
    lineEdit->clear();
}

QString FilterActionRewriteHeader::argsAsString() const
{
    QString result = mParameter;
    result += QLatin1Char('\t');
    result += mRegex.pattern();
    result += QLatin1Char('\t');
    result += mReplacementString;

    return result;
}

QString FilterActionRewriteHeader::displayString() const
{
    return label() + QStringLiteral(" \"") + argsAsString().toHtmlEscaped() + QStringLiteral("\"");
}

void FilterActionRewriteHeader::argsFromString(const QString &argsStr)
{
    const QStringList list = argsStr.split(QLatin1Char('\t'));
    if (list.count() < 3) {
        return;
    }
    QString result;

    result = list[0];
    mRegex.setPattern(list[1]);
    mReplacementString = list[2];

    int index = mParameterList.indexOf(result);
    if (index < 0) {
        mParameterList.append(result);
        index = mParameterList.count() - 1;
    }

    mParameter = mParameterList.at(index);
}

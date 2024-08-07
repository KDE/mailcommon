/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionaddheader.h"

#include <KComboBox>
#include <KLineEdit>
#include <KLocalizedString>

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>

using namespace MailCommon;

FilterActionAddHeader::FilterActionAddHeader(QObject *parent)
    : FilterActionWithStringList(QStringLiteral("add header"), i18n("Add Header"), parent)
{
    mParameterList << QString() << QStringLiteral("Reply-To") << QStringLiteral("Delivered-To") << QStringLiteral("X-KDE-PR-Message")
                   << QStringLiteral("X-KDE-PR-Package") << QStringLiteral("X-KDE-PR-Keywords");

    mParameter = mParameterList.at(0);
}

bool FilterActionAddHeader::isEmpty() const
{
    return mParameter.isEmpty() || mValue.isEmpty();
}

FilterAction::ReturnCode FilterActionAddHeader::process(ItemContext &context, bool) const
{
    if (isEmpty()) {
        return ErrorButGoOn;
    }

    auto msg = context.item().payload<KMime::Message::Ptr>();

    KMime::Headers::Base *header = KMime::Headers::createHeader(mParameter.toLatin1());
    if (!header) {
        header = new KMime::Headers::Generic(mParameter.toLatin1().constData());
    }
    header->fromUnicodeString(mValue);

    msg->setHeader(header);
    msg->assemble();

    context.setNeedsPayloadStore();

    return GoOn;
}

QWidget *FilterActionAddHeader::createParamWidget(QWidget *parent) const
{
    auto widget = new QWidget(parent);
    auto layout = new QHBoxLayout(widget);
    layout->setSpacing(4);
    layout->setContentsMargins({});

    auto comboBox = new KComboBox(widget);
    comboBox->setMinimumWidth(50);
    comboBox->setObjectName(QLatin1StringView("combo"));
    comboBox->setEditable(true);
    comboBox->setInsertPolicy(QComboBox::InsertAtBottom);

    KCompletion *comp = comboBox->completionObject();
    comp->setIgnoreCase(true);
    comp->insertItems(mParameterList);
    comp->setCompletionMode(KCompletion::CompletionPopupAuto);

    layout->addWidget(comboBox, 0 /* stretch */);

    auto label = new QLabel(i18nc("@label:textbox", "With value:"), widget);
    label->setObjectName(QLatin1StringView("label_value"));
    label->setFixedWidth(label->sizeHint().width());
    layout->addWidget(label, 0);

    auto lineEdit = new KLineEdit(widget);
    lineEdit->setObjectName(QLatin1StringView("ledit"));
    lineEdit->setTrapReturnKey(true);
    lineEdit->setClearButtonEnabled(true);
    layout->addWidget(lineEdit, 1);

    setParamWidgetValue(widget);
    connect(comboBox, &KComboBox::currentIndexChanged, this, &FilterActionAddHeader::filterActionModified);
    connect(comboBox->lineEdit(), &QLineEdit::textChanged, this, &FilterAction::filterActionModified);
    connect(lineEdit, &QLineEdit::textChanged, this, &FilterActionAddHeader::filterActionModified);

    return widget;
}

void FilterActionAddHeader::setParamWidgetValue(QWidget *paramWidget) const
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

    auto lineEdit = paramWidget->findChild<QLineEdit *>(QStringLiteral("ledit"));
    Q_ASSERT(lineEdit);

    lineEdit->setText(mValue);
}

void FilterActionAddHeader::applyParamWidgetValue(QWidget *paramWidget)
{
    const auto comboBox = paramWidget->findChild<KComboBox *>(QStringLiteral("combo"));
    Q_ASSERT(comboBox);
    mParameter = comboBox->currentText();

    const QLineEdit *lineEdit = paramWidget->findChild<QLineEdit *>(QStringLiteral("ledit"));
    Q_ASSERT(lineEdit);
    mValue = lineEdit->text();
}

void FilterActionAddHeader::clearParamWidget(QWidget *paramWidget) const
{
    const auto comboBox = paramWidget->findChild<KComboBox *>(QStringLiteral("combo"));
    Q_ASSERT(comboBox);
    comboBox->setCurrentIndex(0);

    auto lineEdit = paramWidget->findChild<QLineEdit *>(QStringLiteral("ledit"));
    Q_ASSERT(lineEdit);
    lineEdit->clear();
}

SearchRule::RequiredPart FilterActionAddHeader::requiredPart() const
{
    return SearchRule::CompleteMessage;
}

QString FilterActionAddHeader::argsAsString() const
{
    QString result = mParameter;
    result += QLatin1Char('\t');
    result += mValue;

    return result;
}

QString FilterActionAddHeader::displayString() const
{
    return label() + QStringLiteral(" \"") + argsAsString().toHtmlEscaped() + QStringLiteral("\"");
}

void FilterActionAddHeader::argsFromString(const QString &argsStr)
{
    const QStringList list = argsStr.split(QLatin1Char('\t'));
    QString result;
    if (list.count() < 2) {
        result = list[0];
        mValue.clear();
    } else {
        result = list[0];
        mValue = list[1];
    }

    int index = mParameterList.indexOf(result);
    if (index < 0) {
        mParameterList.append(result);
        index = mParameterList.count() - 1;
    }

    mParameter = mParameterList.at(index);
}

FilterAction *FilterActionAddHeader::newAction()
{
    return new FilterActionAddHeader;
}

QStringList FilterActionAddHeader::sieveRequires() const
{
    return QStringList() << QStringLiteral("editheader");
}

QString FilterActionAddHeader::sieveCode() const
{
    if (isEmpty()) {
        return QStringLiteral("# invalid filter. Need to fix it by hand");
    } else {
        return QStringLiteral("addheader \"%1\" \"%2\";").arg(mParameter, mValue);
    }
}

QString FilterActionAddHeader::informationAboutNotValidAction() const
{
    QString result;
    if (mParameter.isEmpty()) {
        result = i18n("The header name was missing.");
    }
    if (mValue.isEmpty()) {
        if (result.isEmpty()) {
            result += QLatin1Char('\n');
        }
        result += i18n("The header value was missing.");
    }
    if (!result.isEmpty()) {
        result = name() + QLatin1Char('\n') + result;
    }
    return result;
}

#include "moc_filteractionaddheader.cpp"

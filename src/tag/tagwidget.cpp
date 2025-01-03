/*
  SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "tagwidget.h"

#include <KLineEditEventHandler>

#include <KActionCollection>
#include <KColorCombo>
#include <KIconButton>
#include <KKeySequenceWidget>
#include <KLocalizedString>
#include <QCheckBox>
#include <QGridLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
using namespace MailCommon;
class MailCommon::TagWidgetPrivate
{
public:
    TagWidgetPrivate() = default;

    QLineEdit *mTagNameLineEdit = nullptr;

    QCheckBox *mTextColorCheck = nullptr;
    QCheckBox *mBackgroundColorCheck = nullptr;
    QCheckBox *mTextFontCheck = nullptr;
    QCheckBox *mInToolbarCheck = nullptr;

    KColorCombo *mTextColorCombo = nullptr;
    KColorCombo *mBackgroundColorCombo = nullptr;

    QCheckBox *mBoldCheckBox = nullptr;
    QCheckBox *mItalicCheckBox = nullptr;

    KIconButton *mIconButton = nullptr;

    KKeySequenceWidget *mKeySequenceWidget = nullptr;
};

TagWidget::TagWidget(const QList<KActionCollection *> &actionCollections, QWidget *parent)
    : QWidget(parent)
    , d(new MailCommon::TagWidgetPrivate)
{
    auto settings = new QGridLayout(this);
    settings->setContentsMargins({});

    // Stretcher layout for adding some space after the label
    auto spacer = new QVBoxLayout();
    settings->addLayout(spacer, 0, 0, 1, 2);

    // First row for renaming
    d->mTagNameLineEdit = new QLineEdit(this);
    d->mTagNameLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(d->mTagNameLineEdit);
    settings->addWidget(d->mTagNameLineEdit, 1, 1);

    auto namelabel = new QLabel(i18nc("@label:listbox Name of the tag", "Name:"), this);
    namelabel->setBuddy(d->mTagNameLineEdit);
    settings->addWidget(namelabel, 1, 0);

    connect(d->mTagNameLineEdit, &QLineEdit::textChanged, this, &TagWidget::slotEmitChangeCheck);

    // Second row for text color
    d->mTextColorCheck = new QCheckBox(i18nc("@option:check", "Change te&xt color:"), this);
    settings->addWidget(d->mTextColorCheck, 2, 0);

    d->mTextColorCombo = new KColorCombo(this);
    settings->addWidget(d->mTextColorCombo, 2, 1);
    d->mTextColorCombo->setEnabled(false);

    connect(d->mTextColorCheck, &QCheckBox::toggled, d->mTextColorCombo, &KColorCombo::setEnabled);
    connect(d->mTextColorCheck, &QCheckBox::checkStateChanged, this, &TagWidget::slotEmitChangeCheck);
    connect(d->mTextColorCombo, &KColorCombo::activated, this, &TagWidget::slotEmitChangeCheck);

    // Third row for text background color
    d->mBackgroundColorCheck = new QCheckBox(i18nc("@option:check", "Change &background color:"), this);
    settings->addWidget(d->mBackgroundColorCheck, 3, 0);

    d->mBackgroundColorCombo = new KColorCombo(this);
    settings->addWidget(d->mBackgroundColorCombo, 3, 1);
    d->mBackgroundColorCombo->setEnabled(false);

    connect(d->mBackgroundColorCheck, &QAbstractButton::toggled, d->mBackgroundColorCombo, &QWidget::setEnabled);
    connect(d->mBackgroundColorCheck, &QCheckBox::checkStateChanged, this, &TagWidget::slotEmitChangeCheck);
    connect(d->mBackgroundColorCombo, &KColorCombo::activated, this, &TagWidget::slotEmitChangeCheck);

    // Fourth for font selection
    d->mTextFontCheck = new QCheckBox(i18nc("@option:check", "Change fo&nt:"), this);
    settings->addWidget(d->mTextFontCheck, 4, 0);

    auto fontLayout = new QVBoxLayout;
    settings->addLayout(fontLayout, 4, 1);

    d->mBoldCheckBox = new QCheckBox(i18nc("@option:check", "&Bold"));
    d->mBoldCheckBox->setEnabled(false);
    fontLayout->addWidget(d->mBoldCheckBox);

    d->mItalicCheckBox = new QCheckBox(i18nc("@option:check", "&Italics"));
    d->mItalicCheckBox->setEnabled(false);
    fontLayout->addWidget(d->mItalicCheckBox);

    connect(d->mTextFontCheck, &QAbstractButton::toggled, d->mBoldCheckBox, &QWidget::setEnabled);
    connect(d->mTextFontCheck, &QAbstractButton::toggled, d->mItalicCheckBox, &QWidget::setEnabled);
    connect(d->mTextFontCheck, &QCheckBox::checkStateChanged, this, &TagWidget::slotEmitChangeCheck);
    connect(d->mBoldCheckBox, &QAbstractButton::toggled, this, &TagWidget::slotEmitChangeCheck);
    connect(d->mItalicCheckBox, &QAbstractButton::toggled, this, &TagWidget::slotEmitChangeCheck);

    // Fifth for toolbar icon
    d->mIconButton = new KIconButton(this);
    d->mIconButton->setIconSize(16);
    d->mIconButton->setIconType(KIconLoader::NoGroup, KIconLoader::Action);
    d->mIconButton->setIcon(QIcon::fromTheme(QStringLiteral("mail-tagged")));
    settings->addWidget(d->mIconButton, 5, 1);
    connect(d->mIconButton, &KIconButton::iconChanged, this, &TagWidget::iconNameChanged);

    auto iconlabel = new QLabel(i18nc("@label:textbox", "Message tag &icon:"), this);
    iconlabel->setBuddy(d->mIconButton);
    settings->addWidget(iconlabel, 5, 0);

    // We do not connect the checkbox to icon selector since icons are used in the
    // menus as well
    connect(d->mIconButton, &KIconButton::iconChanged, this, &TagWidget::slotEmitChangeCheck);

    // Sixth for shortcut
    d->mKeySequenceWidget = new KKeySequenceWidget(this);
    settings->addWidget(d->mKeySequenceWidget, 6, 1);
    auto sclabel = new QLabel(i18nc("@label:textbox", "Shortc&ut:"), this);
    sclabel->setBuddy(d->mKeySequenceWidget);
    settings->addWidget(sclabel, 6, 0);
    if (!actionCollections.isEmpty()) {
        d->mKeySequenceWidget->setCheckActionCollections(actionCollections);
        connect(d->mKeySequenceWidget, &KKeySequenceWidget::keySequenceChanged, this, &TagWidget::slotEmitChangeCheck);
    } else {
        d->mKeySequenceWidget->setEnabled(false);
    }

    // Seventh for Toolbar checkbox
    d->mInToolbarCheck = new QCheckBox(i18nc("@option:check", "Enable &toolbar button"), this);
    settings->addWidget(d->mInToolbarCheck, 7, 0);
    connect(d->mInToolbarCheck, &QCheckBox::checkStateChanged, this, &TagWidget::slotEmitChangeCheck);
}

TagWidget::~TagWidget() = default;

void TagWidget::slotEmitChangeCheck()
{
    Q_EMIT changed();
}

void TagWidget::setTagTextColor(const QColor &color)
{
    d->mTextColorCheck->setEnabled(true);
    if (color.isValid()) {
        d->mTextColorCheck->setChecked(true);
        d->mTextColorCombo->setColor(color);
    } else {
        d->mTextColorCheck->setChecked(false);
        d->mTextColorCombo->setColor(Qt::white);
    }
    d->mTextColorCombo->setEnabled(d->mTextColorCheck->isChecked());
}

void TagWidget::setTagBackgroundColor(const QColor &color)
{
    d->mBackgroundColorCheck->setEnabled(true);
    if (color.isValid()) {
        d->mBackgroundColorCheck->setChecked(true);
        d->mBackgroundColorCombo->setColor(color);
    } else {
        d->mBackgroundColorCheck->setChecked(false);
        d->mBackgroundColorCombo->setColor(Qt::white);
    }
    d->mBackgroundColorCombo->setEnabled(d->mBackgroundColorCheck->isChecked());
}

void TagWidget::setTagTextFormat(bool isBold, bool isItalic)
{
    d->mTextFontCheck->setEnabled(true);
    d->mTextFontCheck->setChecked(isBold || isItalic);
    d->mBoldCheckBox->setChecked(isBold);
    d->mItalicCheckBox->setChecked(isItalic);
}

void TagWidget::recordTagSettings(MailCommon::Tag::Ptr tag)
{
    tag->textColor = d->mTextColorCheck->isChecked() ? d->mTextColorCombo->color() : QColor();

    tag->backgroundColor = d->mBackgroundColorCheck->isChecked() ? d->mBackgroundColorCombo->color() : QColor();

    tag->isBold = d->mTextFontCheck->isChecked() ? d->mBoldCheckBox->isChecked() : false;
    tag->isItalic = d->mTextFontCheck->isChecked() ? d->mItalicCheckBox->isChecked() : false;

    tag->iconName = iconButton()->icon();
    if (d->mKeySequenceWidget->isEnabled()) {
        d->mKeySequenceWidget->applyStealShortcut();
        tag->shortcut = QKeySequence(d->mKeySequenceWidget->keySequence());
    }

    tag->inToolbar = d->mInToolbarCheck->isChecked();
}

QLineEdit *TagWidget::tagNameLineEdit() const
{
    return d->mTagNameLineEdit;
}

QCheckBox *TagWidget::textColorCheck() const
{
    return d->mTextColorCheck;
}

QCheckBox *TagWidget::textFontCheck() const
{
    return d->mTextFontCheck;
}

QCheckBox *TagWidget::backgroundColorCheck() const
{
    return d->mBackgroundColorCheck;
}

QCheckBox *TagWidget::inToolBarCheck() const
{
    return d->mInToolbarCheck;
}

KColorCombo *TagWidget::textColorCombo() const
{
    return d->mTextColorCombo;
}

KColorCombo *TagWidget::backgroundColorCombo() const
{
    return d->mBackgroundColorCombo;
}

QCheckBox *TagWidget::textBoldCheck() const
{
    return d->mBoldCheckBox;
}

QCheckBox *TagWidget::textItalicCheck() const
{
    return d->mItalicCheckBox;
}

KIconButton *TagWidget::iconButton() const
{
    return d->mIconButton;
}

KKeySequenceWidget *TagWidget::keySequenceWidget() const
{
    return d->mKeySequenceWidget;
}

#include "moc_tagwidget.cpp"

/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "collectionviewwidget.h"
#include "mailcommon_debug.h"
#include <KLocalizedString>
#include <QVBoxLayout>

#include <Akonadi/KMime/MessageFolderAttribute>
#include <MessageList/AggregationComboBox>
#include <MessageList/AggregationConfigButton>
#include <MessageList/ThemeComboBox>
#include <MessageList/ThemeConfigButton>

#include <MessageViewer/Viewer>

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QRadioButton>
using namespace MailCommon;
CollectionViewWidget::CollectionViewWidget(QWidget *parent)
    : QWidget(parent)
{
    auto topLayout = new QVBoxLayout(this);
    topLayout->setObjectName(QStringLiteral("topLayout"));
    topLayout->setContentsMargins({});

    // sender or receiver column
    const QString senderReceiverColumnTip = i18n("Show Sender/Receiver Column in List of Messages");

    auto senderReceiverColumnLabel = new QLabel(i18n("Sho&w column:"), this);
    mShowSenderReceiverComboBox = new QComboBox(this);
    mShowSenderReceiverComboBox->setToolTip(senderReceiverColumnTip);
    senderReceiverColumnLabel->setBuddy(mShowSenderReceiverComboBox);
    mShowSenderReceiverComboBox->insertItem(0, i18nc("@item:inlistbox Show default value.", "Default"));
    mShowSenderReceiverComboBox->insertItem(1, i18nc("@item:inlistbox Show sender.", "Sender"));
    mShowSenderReceiverComboBox->insertItem(2, i18nc("@item:inlistbox Show receiver.", "Receiver"));

    auto senderReceiverColumnHLayout = new QHBoxLayout();
    senderReceiverColumnHLayout->addWidget(senderReceiverColumnLabel);
    senderReceiverColumnHLayout->addWidget(mShowSenderReceiverComboBox);
    topLayout->addLayout(senderReceiverColumnHLayout);

    // message list
    auto messageListGroup = new QGroupBox(i18n("Message List"), this);
    auto messageListGroupLayout = new QVBoxLayout(messageListGroup);
    topLayout->addWidget(messageListGroup);

    // message list aggregation
    mUseDefaultAggregationCheckBox = new QCheckBox(i18n("Use default aggregation"), messageListGroup);
    messageListGroupLayout->addWidget(mUseDefaultAggregationCheckBox);
    connect(mUseDefaultAggregationCheckBox, &QCheckBox::stateChanged, this, &CollectionViewWidget::slotAggregationCheckboxChanged);

    mAggregationComboBox = new MessageList::Utils::AggregationComboBox(messageListGroup);

    auto aggregationLabel = new QLabel(i18n("Aggregation"), messageListGroup);
    aggregationLabel->setBuddy(mAggregationComboBox);

    using MessageList::Utils::AggregationConfigButton;
    auto aggregationConfigButton = new AggregationConfigButton(messageListGroup, mAggregationComboBox);
    // Make sure any changes made in the aggregations configure dialog are reflected in the combo.
    connect(aggregationConfigButton, &AggregationConfigButton::configureDialogCompleted, this, &CollectionViewWidget::slotSelectFolderAggregation);

    auto aggregationLayout = new QHBoxLayout();
    aggregationLayout->addWidget(aggregationLabel, 1);
    aggregationLayout->addWidget(mAggregationComboBox, 1);
    aggregationLayout->addWidget(aggregationConfigButton, 0);
    messageListGroupLayout->addLayout(aggregationLayout);

    // message list theme
    mUseDefaultThemeCheckBox = new QCheckBox(i18n("Use default theme"), messageListGroup);
    messageListGroupLayout->addWidget(mUseDefaultThemeCheckBox);
    connect(mUseDefaultThemeCheckBox, &QCheckBox::stateChanged, this, &CollectionViewWidget::slotThemeCheckboxChanged);

    mThemeComboBox = new MessageList::Utils::ThemeComboBox(messageListGroup);

    auto themeLabel = new QLabel(i18n("Theme"), messageListGroup);
    themeLabel->setBuddy(mThemeComboBox);

    using MessageList::Utils::ThemeConfigButton;
    auto themeConfigButton = new ThemeConfigButton(messageListGroup, mThemeComboBox);
    // Make sure any changes made in the themes configure dialog are reflected in the combo.
    connect(themeConfigButton, &ThemeConfigButton::configureDialogCompleted, this, &CollectionViewWidget::slotSelectFolderTheme);

    auto themeLayout = new QHBoxLayout();
    themeLayout->addWidget(themeLabel, 1);
    themeLayout->addWidget(mThemeComboBox, 1);
    themeLayout->addWidget(themeConfigButton, 0);
    messageListGroupLayout->addLayout(themeLayout);

    // Message Default Format
    auto messageFormatGroup = new QGroupBox(i18n("Message Default Format"), this);
    auto messageFormatGroupLayout = new QVBoxLayout(messageFormatGroup);
    mPreferHtmlToText = new QRadioButton(i18n("Prefer HTML to text"), this);
    messageFormatGroupLayout->addWidget(mPreferHtmlToText);
    mPreferTextToHtml = new QRadioButton(i18n("Prefer text to HTML"), this);
    messageFormatGroupLayout->addWidget(mPreferTextToHtml);
    mUseGlobalSettings = new QRadioButton(i18n("Use Global Settings"), this);
    messageFormatGroupLayout->addWidget(mUseGlobalSettings);

    topLayout->addWidget(messageFormatGroup);

    topLayout->addStretch(100);
}

CollectionViewWidget::~CollectionViewWidget()
{
}

void CollectionViewWidget::load(const Akonadi::Collection &col)
{
    mCurrentCollection = col;
    mFolderCollection = MailCommon::FolderSettings::forCollection(col);
    if (col.hasAttribute<Akonadi::MessageFolderAttribute>()) {
        const bool outboundFolder = col.attribute<Akonadi::MessageFolderAttribute>()->isOutboundFolder();
        if (outboundFolder) {
            mShowSenderReceiverComboBox->setCurrentIndex(2);
        } else {
            mShowSenderReceiverComboBox->setCurrentIndex(1);
        }
    } else {
        mShowSenderReceiverComboBox->setCurrentIndex(0);
    }
    mShowSenderReceiverValue = mShowSenderReceiverComboBox->currentIndex();

    // message list aggregation
    slotSelectFolderAggregation();

    // message list theme
    slotSelectFolderTheme();

    const MessageViewer::Viewer::DisplayFormatMessage formatMessage = mFolderCollection->formatMessage();
    switch (formatMessage) {
    case MessageViewer::Viewer::Html:
        mPreferHtmlToText->setChecked(true);
        break;
    case MessageViewer::Viewer::Text:
        mPreferTextToHtml->setChecked(true);
        break;
    case MessageViewer::Viewer::UseGlobalSetting:
        mUseGlobalSettings->setChecked(true);
        break;
    default:
        qCDebug(MAILCOMMON_LOG) << "No settings defined";
        break;
    }
}

void CollectionViewWidget::save(Akonadi::Collection &col)
{
    if (!mFolderCollection) {
        mFolderCollection = MailCommon::FolderSettings::forCollection(col);
    }
    const int currentIndex = mShowSenderReceiverComboBox->currentIndex();
    if (mShowSenderReceiverValue != currentIndex) {
        if (currentIndex == 1) {
            auto *messageFolder = col.attribute<Akonadi::MessageFolderAttribute>(Akonadi::Collection::AddIfMissing);
            messageFolder->setOutboundFolder(false);
        } else if (currentIndex == 2) {
            auto *messageFolder = col.attribute<Akonadi::MessageFolderAttribute>(Akonadi::Collection::AddIfMissing);
            messageFolder->setOutboundFolder(true);
        } else {
            col.removeAttribute<Akonadi::MessageFolderAttribute>();
        }
    }
    // message list theme
    const bool usePrivateTheme = !mUseDefaultThemeCheckBox->isChecked();
    mThemeComboBox->writeStorageModelConfig(QString::number(mCurrentCollection.id()), usePrivateTheme);
    // message list aggregation
    const bool usePrivateAggregation = !mUseDefaultAggregationCheckBox->isChecked();
    mAggregationComboBox->writeStorageModelConfig(QString::number(mCurrentCollection.id()), usePrivateAggregation);

    MessageViewer::Viewer::DisplayFormatMessage formatMessage = MessageViewer::Viewer::Unknown;
    if (mPreferHtmlToText->isChecked()) {
        formatMessage = MessageViewer::Viewer::Html;
    } else if (mPreferTextToHtml->isChecked()) {
        formatMessage = MessageViewer::Viewer::Text;
    } else if (mUseGlobalSettings->isChecked()) {
        formatMessage = MessageViewer::Viewer::UseGlobalSetting;
    } else {
        qCDebug(MAILCOMMON_LOG) << "No settings defined";
    }
    if (formatMessage != MessageViewer::Viewer::Unknown) {
        mFolderCollection->setFormatMessage(formatMessage);
        mFolderCollection->writeConfig();
    }
}

void CollectionViewWidget::slotSelectFolderAggregation()
{
    bool usesPrivateAggregation = false;
    mAggregationComboBox->readStorageModelConfig(mCurrentCollection, usesPrivateAggregation);
    mUseDefaultAggregationCheckBox->setChecked(!usesPrivateAggregation);
}

void CollectionViewWidget::slotSelectFolderTheme()
{
    bool usesPrivateTheme = false;
    mThemeComboBox->readStorageModelConfig(mCurrentCollection, usesPrivateTheme);
    mUseDefaultThemeCheckBox->setChecked(!usesPrivateTheme);
}

void CollectionViewWidget::slotAggregationCheckboxChanged()
{
    mAggregationComboBox->setEnabled(!mUseDefaultAggregationCheckBox->isChecked());
}

void CollectionViewWidget::slotThemeCheckboxChanged()
{
    mThemeComboBox->setEnabled(!mUseDefaultThemeCheckBox->isChecked());
}

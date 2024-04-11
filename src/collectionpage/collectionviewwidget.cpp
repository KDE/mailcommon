/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "collectionviewwidget.h"
#include "mailcommon_debug.h"
#include <KLocalizedString>
#include <QFormLayout>

#include <Akonadi/MessageFolderAttribute>
#include <MessageList/AggregationComboBox>
#include <MessageList/AggregationConfigButton>
#include <MessageList/ThemeComboBox>
#include <MessageList/ThemeConfigButton>

#include <MessageViewer/Viewer>

#include <QCheckBox>
#include <QComboBox>
#include <QRadioButton>
using namespace MailCommon;
CollectionViewWidget::CollectionViewWidget(QWidget *parent)
    : QWidget(parent)
    , mShowSenderReceiverComboBox(new QComboBox(this))
    , mUseDefaultAggregationCheckBox(new QCheckBox(i18n("Use default message list aggregation:"), this))
    , mAggregationComboBox(new MessageList::Utils::AggregationComboBox(this))
    , mUseDefaultThemeCheckBox(new QCheckBox(i18n("Use default message list theme"), this))
    , mThemeComboBox(new MessageList::Utils::ThemeComboBox(this))
    , mPreferHtmlToText(new QRadioButton(i18n("Prefer HTML to text"), this))
    , mPreferTextToHtml(new QRadioButton(i18n("Prefer text to HTML"), this))
    , mUseGlobalSettings(new QRadioButton(i18n("Use Global Settings"), this))
{
    auto topLayout = new QFormLayout(this);
    topLayout->setObjectName(QLatin1StringView("topLayout"));
    topLayout->setContentsMargins({});

    // sender or receiver column
    const QString senderReceiverColumnTip = i18n("Show Sender/Receiver Column in List of Messages");

    mShowSenderReceiverComboBox->setToolTip(senderReceiverColumnTip);
    mShowSenderReceiverComboBox->insertItem(0, i18nc("@item:inlistbox Show default value.", "Default"));
    mShowSenderReceiverComboBox->insertItem(1, i18nc("@item:inlistbox Show sender.", "Sender"));
    mShowSenderReceiverComboBox->insertItem(2, i18nc("@item:inlistbox Show receiver.", "Receiver"));
    topLayout->addRow(i18n("Sho&w column:"), mShowSenderReceiverComboBox);

    // message list aggregation
#if QT_VERSION < QT_VERSION_CHECK(6, 7, 0)
    connect(mUseDefaultAggregationCheckBox, &QCheckBox::stateChanged, this, &CollectionViewWidget::slotAggregationCheckboxChanged);
#else
    connect(mUseDefaultAggregationCheckBox, &QCheckBox::checkStateChanged, this, &CollectionViewWidget::slotAggregationCheckboxChanged);
#endif
    topLayout->addRow(QString(), mUseDefaultAggregationCheckBox);

    using MessageList::Utils::AggregationConfigButton;
    auto aggregationConfigButton = new AggregationConfigButton(this, mAggregationComboBox);
    // Make sure any changes made in the aggregations configure dialog are reflected in the combo.
    connect(aggregationConfigButton, &AggregationConfigButton::configureDialogCompleted, this, &CollectionViewWidget::slotSelectFolderAggregation);

    auto aggregationLayout = new QHBoxLayout;
    aggregationLayout->addWidget(mAggregationComboBox, 1);
    aggregationLayout->addWidget(aggregationConfigButton, 0);
    topLayout->addRow(QString(), aggregationLayout);

    // message list theme
#if QT_VERSION < QT_VERSION_CHECK(6, 7, 0)
    connect(mUseDefaultThemeCheckBox, &QCheckBox::stateChanged, this, &CollectionViewWidget::slotThemeCheckboxChanged);
#else
    connect(mUseDefaultThemeCheckBox, &QCheckBox::checkStateChanged, this, &CollectionViewWidget::slotThemeCheckboxChanged);
#endif
    topLayout->addRow(QString(), mUseDefaultThemeCheckBox);

    using MessageList::Utils::ThemeConfigButton;
    auto themeConfigButton = new ThemeConfigButton(this, mThemeComboBox);
    // Make sure any changes made in the themes configure dialog are reflected in the combo.
    connect(themeConfigButton, &ThemeConfigButton::configureDialogCompleted, this, &CollectionViewWidget::slotSelectFolderTheme);

    auto themeLayout = new QHBoxLayout;
    themeLayout->addWidget(mThemeComboBox, 1);
    themeLayout->addWidget(themeConfigButton, 0);
    topLayout->addRow(QString(), themeLayout);

    // Message Default Format
    topLayout->addRow(i18n("Message format:"), mPreferHtmlToText);
    topLayout->addRow(QString(), mPreferTextToHtml);
    topLayout->addRow(QString(), mUseGlobalSettings);
}

CollectionViewWidget::~CollectionViewWidget() = default;

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
    mFolderCollection.reset();
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

#include "moc_collectionviewwidget.cpp"

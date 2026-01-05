/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <MailCommon/FolderSettings>
#include <QSharedPointer>
#include <QWidget>
class QCheckBox;
class QComboBox;
class QRadioButton;
namespace MessageList
{
namespace Utils
{
class AggregationComboBox;
class ThemeComboBox;
}
}
namespace MailCommon
{
/*!
 * \class MailCommon::CollectionViewWidget
 * \inmodule MailCommon
 * \inheaderfile MailCommon/CollectionViewWidget
 *
 * \brief The CollectionViewWidget class
 * \author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_EXPORT CollectionViewWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     */
    explicit CollectionViewWidget(QWidget *parent = nullptr);
    /*!
     */
    ~CollectionViewWidget() override;
    /*!
     */
    void load(const Akonadi::Collection &col);
    /*!
     */
    void save(Akonadi::Collection &col);

private:
    MAILCOMMON_NO_EXPORT void slotSelectFolderAggregation();
    MAILCOMMON_NO_EXPORT void slotSelectFolderTheme();
    MAILCOMMON_NO_EXPORT void slotThemeCheckboxChanged();
    MAILCOMMON_NO_EXPORT void slotAggregationCheckboxChanged();
    QSharedPointer<MailCommon::FolderSettings> mFolderCollection;
    QComboBox *const mShowSenderReceiverComboBox;
    QCheckBox *const mUseDefaultAggregationCheckBox;
    MessageList::Utils::AggregationComboBox *const mAggregationComboBox;
    QCheckBox *const mUseDefaultThemeCheckBox;
    MessageList::Utils::ThemeComboBox *const mThemeComboBox;
    QRadioButton *const mPreferHtmlToText;
    QRadioButton *const mPreferTextToHtml;
    QRadioButton *const mUseGlobalSettings;
    Akonadi::Collection mCurrentCollection;
    int mShowSenderReceiverValue = -1;
};
}

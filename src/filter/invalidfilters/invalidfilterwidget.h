/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_private_export.h"
#include <QWidget>

namespace MailCommon
{
class InvalidFilterInfo;
class InvalidFilterListView;
/*!
 * \class MailCommon::InvalidFilterWidget
 * \inmodule MailCommon
 * \inheaderfile MailCommon/InvalidFilterWidget
 *
 * \brief The InvalidFilterWidget class.
 *
 * \author Laurent Montel <montel@kde.org>
 */
class MAILCOMMON_TESTS_EXPORT InvalidFilterWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructs an invalid filter widget with the given \a parent.
     */
    explicit InvalidFilterWidget(QWidget *parent = nullptr);
    /*!
     * Destroys the widget.
     */
    ~InvalidFilterWidget() override;

    /*!
     * Sets the list of invalid filters \a lst to display.
     */
    void setInvalidFilters(const QList<MailCommon::InvalidFilterInfo> &lst);
Q_SIGNALS:
    /*!
     * Emitted to show the details of an invalid filter.
     */
    void showDetails(const QString &);
    /*!
     * Emitted to hide the information widget.
     */
    void hideInformationWidget();

private:
    InvalidFilterListView *const mInvalidFilterListWidget;
};
}

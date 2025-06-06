/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

#include "filteractionwithurl.h"
#include "mailcommon_private_export.h"
#include <QList>
class QTemporaryFile;

namespace MailCommon
{
class MAILCOMMON_TESTS_EXPORT FilterActionWithCommand : public FilterActionWithUrl
{
    Q_OBJECT

public:
    /**
     * @copydoc FilterAction::FilterAction
     */
    FilterActionWithCommand(const QString &name, const QString &label, QObject *parent = nullptr);

    /**
     * @copydoc FilterAction::createParamWidget
     */
    QWidget *createParamWidget(QWidget *parent) const override;

    /**
     * @copydoc FilterAction::applyParamWidgetValue
     */
    void applyParamWidgetValue(QWidget *paramWidget) override;

    /**
     * @copydoc FilterAction::setParamWidgetValue
     */
    void setParamWidgetValue(QWidget *paramWidget) const override;

    /**
     * @copydoc FilterAction::clearParamWidget
     */
    void clearParamWidget(QWidget *paramWidget) const override;

    /**
     * Substitutes various placeholders for data from the message
     * resp. for filenames containing that data. Currently, only %n is
     * supported, where n in an integer >= 0. %n gets substituted for
     * the name of a tempfile holding the n'th message part, with n=0
     * meaning the body of the message.
     */
    [[nodiscard]] virtual QString substituteCommandLineArgsFor(const KMime::Message::Ptr &aMsg, QList<QTemporaryFile *> &aTempFileList) const;

    [[nodiscard]] virtual ReturnCode genericProcess(ItemContext &context, bool filtering) const;
};
}

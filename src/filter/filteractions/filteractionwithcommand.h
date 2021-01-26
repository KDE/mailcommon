/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONWITHCOMMAND_H
#define MAILCOMMON_FILTERACTIONWITHCOMMAND_H

#include "filteractionwithurl.h"
#include <QVector>
class QTemporaryFile;

namespace MailCommon
{
class FilterActionWithCommand : public FilterActionWithUrl
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
    Q_REQUIRED_RESULT virtual QString substituteCommandLineArgsFor(const KMime::Message::Ptr &aMsg, QVector<QTemporaryFile *> &aTempFileList) const;

    Q_REQUIRED_RESULT virtual ReturnCode genericProcess(ItemContext &context, bool filtering) const;
};
}

#endif

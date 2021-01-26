/*  -*- mode: C++ -*-

  SPDX-FileCopyrightText: 2003 Andreas Gungl <a.gungl@gmx.de>
  SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/
#ifndef MAILCOMMON_REDIRECTDIALOG_H
#define MAILCOMMON_REDIRECTDIALOG_H

#include "mailcommon_export.h"

#include <QDialog>

namespace MailCommon
{
/**
 * @short A dialog to request information about message redirection from the user.
 *
 * The dialog is used to collect redirect addresses when
 * manually redirecting messages. Only Redirect-To is
 * supported so far.
 *
 * @author Andreas Gungl <a.gungl@gmx.de>
 */
class MAILCOMMON_EXPORT RedirectDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * Describes the send mode.
     */
    enum SendMode { SendNow, SendLater };

    /**
     * Creates a new redirect dialog.
     *
     * @param mode The preferred send mode.
     * @param parent The parent widget.
     */
    explicit RedirectDialog(SendMode mode = SendNow, QWidget *parent = nullptr);

    /**
     * Destroys the redirect dialog.
     */
    ~RedirectDialog() override;

    /**
     * Returns the addresses for the redirection.
     */
    Q_REQUIRED_RESULT QString to() const;

    /**
     * Returns the send mode.
     */
    Q_REQUIRED_RESULT SendMode sendMode() const;

    Q_REQUIRED_RESULT int transportId() const;

    Q_REQUIRED_RESULT int identity() const;

    Q_REQUIRED_RESULT QString cc() const;
    Q_REQUIRED_RESULT QString bcc() const;

protected:
    void accept() override;

private:
    //@cond PRIVATE
    class Private;
    Private *const d;
    //@endcond
};
}

#endif

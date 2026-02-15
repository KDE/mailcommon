/*  -*- mode: C++ -*-

  SPDX-FileCopyrightText: 2003 Andreas Gungl <a.gungl@gmx.de>
  SPDX-FileCopyrightText: 2014-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/
#pragma once

#include "mailcommon_export.h"

#include <QDialog>

namespace MailCommon
{
/*!
 * \class MailCommon::RedirectDialog
 * \inmodule MailCommon
 * \inheaderfile MailCommon/RedirectDialog
 *
 * \brief A dialog to request information about message redirection from the user.
 *
 * The dialog is used to collect redirect addresses when
 * manually redirecting messages. Only Redirect-To is
 * supported so far.
 *
 * \author Andreas Gungl <a.gungl@gmx.de>
 */
class MAILCOMMON_EXPORT RedirectDialog : public QDialog
{
    Q_OBJECT

public:
    /*!
     * Describes the send mode.
     */
    enum SendMode {
        SendNow,
        SendLater,
    };

    /*!
     * Creates a new redirect dialog.
     *
     * \a mode The preferred send mode.
     * \a parent The parent widget.
     */
    explicit RedirectDialog(SendMode mode = SendNow, QWidget *parent = nullptr);

    /*!
     * Destroys the redirect dialog.
     */
    ~RedirectDialog() override;

    /*!
     * Returns the addresses for the redirection.
     */
    [[nodiscard]] QString to() const;

    /*!
     * Returns the send mode.
     */
    [[nodiscard]] SendMode sendMode() const;

    /*!
     * Returns the transport ID selected in the dialog.
     */
    [[nodiscard]] int transportId() const;

    /*!
     * Returns the identity ID selected in the dialog.
     */
    [[nodiscard]] int identity() const;

    /*!
     * Returns the CC addresses for the redirection.
     */
    [[nodiscard]] QString cc() const;
    /*!
     * Returns the BCC addresses for the redirection.
     */
    [[nodiscard]] QString bcc() const;

protected:
    /*!
     * Accepts the dialog and stores the user's settings.
     */
    void accept() override;

private:
    class RedirectDialogPrivate;
    std::unique_ptr<RedirectDialogPrivate> const d;
};
}

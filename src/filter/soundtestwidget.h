/*
  kmfawidgets.h - KMFilterAction parameter widgets
  SPDX-FileCopyrightText: 2001 Marc Mutz <mutz@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_private_export.h"
#include <QWidget>
class QMediaPlayer;
class QAudioOutput;
class QPushButton;
class KUrlRequester;

namespace MailCommon
{
/**
 * @short A widget to play a sound from a given URL.
 */
class MAILCOMMON_TESTS_EXPORT SoundTestWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Creates a new sound test widget.
     *
     * @param parent The parent widget.
     */
    explicit SoundTestWidget(QWidget *parent = nullptr);

    /**
     * Destroys the sound test widget.
     */
    ~SoundTestWidget() override;

    /**
     * Sets the @p url of the sound file to play.
     */
    void setUrl(const QString &url);

    /**
     * Returns the url of the sound file to play.
     */
    [[nodiscard]] QString url() const;

    /**
     * Clears the url of the sound file to play.
     */
    void clear();

Q_SIGNALS:
    /**
     * This signal is emitted when the user
     * enters a new URL.
     */
    void textChanged(const QString &);

private:
    void playSound();
    void openSoundDialog(KUrlRequester *);
    void slotUrlChanged(const QString &);
    KUrlRequester *const m_urlRequester;
    QPushButton *const m_playButton;
    QMediaPlayer *m_player = nullptr;
    QAudioOutput *m_output = nullptr;
};
}

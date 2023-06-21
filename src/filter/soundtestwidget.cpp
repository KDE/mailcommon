/*
  SPDX-FileCopyrightText: 2001 Marc Mutz <mutz@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "soundtestwidget.h"

#include <KLineEdit>
#include <KLocalizedString>
#include <KUrlRequester>
#include <QIcon>
#include <QUrl>

#include <QHBoxLayout>
#include <QPushButton>

#include <QStandardPaths>
#include <phonon/backendcapabilities.h>
#include <phonon/mediaobject.h>

using namespace MailCommon;

SoundTestWidget::SoundTestWidget(QWidget *parent)
    : QWidget(parent)
    , m_urlRequester(new KUrlRequester(this))
    , m_playButton(new QPushButton(this))
{
    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins({});

    m_playButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
    m_playButton->setToolTip(i18n("Play"));
    layout->addWidget(m_playButton);

    layout->addWidget(m_urlRequester);

    connect(m_playButton, &QPushButton::clicked, this, &SoundTestWidget::playSound);
    connect(m_urlRequester, &KUrlRequester::openFileDialog, this, &SoundTestWidget::openSoundDialog);
    connect(m_urlRequester->lineEdit(), &QLineEdit::textChanged, this, &SoundTestWidget::slotUrlChanged);

    slotUrlChanged(m_urlRequester->lineEdit()->text());
}

SoundTestWidget::~SoundTestWidget() = default;

void SoundTestWidget::slotUrlChanged(const QString &url)
{
    m_playButton->setEnabled(!url.trimmed().isEmpty());
    Q_EMIT textChanged(url);
}

void SoundTestWidget::openSoundDialog(KUrlRequester *)
{
    static bool init = true;
    if (!init) {
        return;
    }

    init = false;
    QFileDialog *fileDialog = m_urlRequester->fileDialog();
    fileDialog->setWindowTitle(i18nc("@title:window", "Select Sound File"));

    m_urlRequester->setMimeTypeFilters(Phonon::BackendCapabilities::availableMimeTypes());

    const QStringList soundDirs = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, QStringLiteral("sound/"), QStandardPaths::LocateDirectory);

    if (!soundDirs.isEmpty()) {
        QUrl soundURL;
        QDir dir;
        dir.setFilter(QDir::Files | QDir::Readable);

        for (const QString &soundDir : soundDirs) {
            dir = soundDir;
            if (dir.isReadable() && dir.count() > 2) {
                soundURL.setPath(soundDir);
                m_urlRequester->setUrl(soundURL);
                break;
            }
        }
    }
}

void SoundTestWidget::playSound()
{
    const QString parameter = m_urlRequester->lineEdit()->text();
    if (parameter.isEmpty()) {
        return;
    }

    const QString file = QStringLiteral("file:");
    const QString play = (parameter.startsWith(file) ? parameter.mid(file.length()) : parameter);
    if (m_player) {
        if (m_player->state() == Phonon::PlayingState) {
            m_player->pause();
        } else {
            m_player->setCurrentSource(QUrl::fromLocalFile(play));
            m_player->play();
        }
    } else {
        m_player = Phonon::createPlayer(Phonon::NotificationCategory, QUrl::fromLocalFile(play));
        m_player->setParent(this);
        m_player->play();
        connect(m_player, &Phonon::MediaObject::stateChanged, this, [this](Phonon::State newState, Phonon::State) {
            if (newState == Phonon::PlayingState) {
                m_playButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-pause")));
                m_playButton->setToolTip(i18n("Pause"));
            } else {
                m_playButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
                m_playButton->setToolTip(i18n("Play"));
            }
        });
    }
}

QString SoundTestWidget::url() const
{
    return m_urlRequester->lineEdit()->text();
}

void SoundTestWidget::setUrl(const QString &url)
{
    m_urlRequester->lineEdit()->setText(url);
}

void SoundTestWidget::clear()
{
    m_urlRequester->lineEdit()->clear();
}

#include "moc_soundtestwidget.cpp"

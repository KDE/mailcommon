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
#include <phonon/mediaobject.h>

using namespace MailCommon;

SoundTestWidget::SoundTestWidget(QWidget *parent)
    : QWidget(parent)
{
    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_playButton = new QPushButton(this);
    m_playButton->setIcon(QIcon::fromTheme(QStringLiteral("arrow-right")));
    m_playButton->setToolTip(i18n("Play"));
    layout->addWidget(m_playButton);

    m_urlRequester = new KUrlRequester(this);
    layout->addWidget(m_urlRequester);

    connect(m_playButton, &QPushButton::clicked, this, &SoundTestWidget::playSound);
    connect(m_urlRequester, &KUrlRequester::openFileDialog, this, &SoundTestWidget::openSoundDialog);
    connect(m_urlRequester->lineEdit(), &QLineEdit::textChanged, this, &SoundTestWidget::slotUrlChanged);

    slotUrlChanged(m_urlRequester->lineEdit()->text());
}

SoundTestWidget::~SoundTestWidget()
{
}

void SoundTestWidget::slotUrlChanged(const QString &url)
{
    m_playButton->setEnabled(!url.isEmpty());
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

    const QStringList filters{QStringLiteral("audio/x-wav"), QStringLiteral("application/ogg"), QStringLiteral("audio/x-adpcm")};

    m_urlRequester->setMimeTypeFilters(filters);

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
    Phonon::MediaObject *player = Phonon::createPlayer(Phonon::NotificationCategory, QUrl::fromLocalFile(play));
    player->play();
    connect(player, &Phonon::MediaObject::finished, player, &Phonon::MediaObject::deleteLater);
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

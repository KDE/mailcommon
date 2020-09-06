/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionplaysound.h"

#include "filter/dialog/filteractionmissingsoundurldialog.h"

#include <phonon/mediaobject.h>

#include <KLocalizedString>

#include <QFile>
#include <QPointer>

using namespace MailCommon;

FilterActionPlaySound::FilterActionPlaySound()
    : FilterActionWithTest(QStringLiteral("play sound"), i18n("Play Sound"))
{
}

FilterActionPlaySound::~FilterActionPlaySound()
{
    delete mPlayer;
}

FilterAction *FilterActionPlaySound::newAction()
{
    return new FilterActionPlaySound();
}

bool FilterActionPlaySound::isEmpty() const
{
    return mParameter.isEmpty();
}

FilterAction::ReturnCode FilterActionPlaySound::process(ItemContext &, bool) const
{
    if (isEmpty()) {
        return ErrorButGoOn;
    }
    if (!mPlayer) {
        mPlayer = Phonon::createPlayer(Phonon::NotificationCategory);
    }

    mPlayer->setCurrentSource(mParameter);
    mPlayer->play();
    return GoOn;
}

SearchRule::RequiredPart FilterActionPlaySound::requiredPart() const
{
    return SearchRule::Envelope;
}

bool FilterActionPlaySound::argsFromStringInteractive(const QString &argsStr, const QString &filterName)
{
    bool needUpdate = false;
    argsFromString(argsStr);
    if (!QFile(mParameter).exists()) {
        QPointer<MailCommon::FilterActionMissingSoundUrlDialog> dlg = new MailCommon::FilterActionMissingSoundUrlDialog(filterName, argsStr);
        if (dlg->exec()) {
            mParameter = dlg->soundUrl();
            needUpdate = true;
        }
        delete dlg;
    }
    return needUpdate;
}

QString FilterActionPlaySound::informationAboutNotValidAction() const
{
    return i18n("Sound file was not defined.");
}

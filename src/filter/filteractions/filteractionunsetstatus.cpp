/*

  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filteractionunsetstatus.h"
#include "mailcommon_debug.h"
#include <KLocalizedString>
using namespace MailCommon;

FilterAction *FilterActionUnsetStatus::newAction()
{
    return new FilterActionUnsetStatus;
}

FilterActionUnsetStatus::FilterActionUnsetStatus(QObject *parent)
    : FilterActionStatus(QStringLiteral("unset status"), i18nc("action: to unset the status", "Unset Status"), parent)
{
}

bool FilterActionUnsetStatus::checkIsValid(int &index) const
{
    index = mParameterList.indexOf(mParameter);
    return index < 1;
}

bool FilterActionUnsetStatus::isEmpty() const
{
    int index = 0;
    return checkIsValid(index);
}

QString FilterActionUnsetStatus::informationAboutNotValidAction() const
{
    return i18n("Status not defined.");
}

FilterAction::ReturnCode FilterActionUnsetStatus::process(ItemContext &context, bool) const
{
    int index = 0;
    if (checkIsValid(index)) {
        return ErrorButGoOn;
    }

    Akonadi::MessageStatus status;
    status.setStatusFromFlags(context.item().flags());

    Akonadi::MessageStatus newStatus = FilterActionStatus::stati[index - 1];
    // Laurent unread status doesn't exist. And crash if we use "const Akonadi::Item::Flag flag = *(flags.begin());" etc. => if we unset "unread" we set read
    if (newStatus == Akonadi::MessageStatus::statusUnread()) {
        Akonadi::MessageStatus oldStatus = status;
        newStatus.setRead(true);
        if (oldStatus != newStatus) {
            context.item().setFlags(newStatus.statusFlags());
            context.setNeedsFlagStore();
        }
        return GoOn;
    }

    QSet<QByteArray> flags = newStatus.statusFlags();
    const Akonadi::Item::Flag flag = *(flags.begin());
    if (context.item().hasFlag(flag)) {
        context.item().clearFlag(flag);
        context.setNeedsFlagStore();
    }
    return GoOn;
}

SearchRule::RequiredPart FilterActionUnsetStatus::requiredPart() const
{
    return SearchRule::Envelope;
}

QString FilterActionUnsetStatus::sieveCode() const
{
    QString flagCode;
    const QString parameter = argsAsString();
    if (parameter == QLatin1Char('R')) {
        flagCode = QStringLiteral("\\\\Seen");
    } else if (parameter == QLatin1Char('D')) {
        flagCode = QStringLiteral("\\\\Deleted");
    } else if (parameter == QLatin1Char('A')) {
        flagCode = QStringLiteral("\\\\Answered");
    } else {
        qCDebug(MAILCOMMON_LOG) << " FilterActionSetStatus::sieveCode() unknown flags" << parameter;
        flagCode = parameter;
    }

    const QString result = QStringLiteral("removeflag \"%1\";").arg(flagCode);
    return result;
}

QStringList FilterActionUnsetStatus::sieveRequires() const
{
    return QStringList() << QStringLiteral("imap4flags");
}

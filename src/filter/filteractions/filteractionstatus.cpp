/*

  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filteractionstatus.h"
#include <KLocalizedString>
using namespace MailCommon;

const Akonadi::MessageStatus MailCommon::FilterActionStatus::stati[] = {Akonadi::MessageStatus::statusImportant(),
                                                                        Akonadi::MessageStatus::statusRead(),
                                                                        Akonadi::MessageStatus::statusUnread(),
                                                                        Akonadi::MessageStatus::statusReplied(),
                                                                        Akonadi::MessageStatus::statusForwarded(),
                                                                        Akonadi::MessageStatus::statusWatched(),
                                                                        Akonadi::MessageStatus::statusIgnored(),
                                                                        Akonadi::MessageStatus::statusSpam(),
                                                                        Akonadi::MessageStatus::statusHam(),
                                                                        Akonadi::MessageStatus::statusToAct()};

const int MailCommon::FilterActionStatus::StatiCount = sizeof(MailCommon::FilterActionStatus::stati) / sizeof(Akonadi::MessageStatus);

FilterActionStatus::FilterActionStatus(const QString &name, const QString &label, QObject *parent)
    : FilterActionWithStringList(name, label, parent)
{
    // if you change this list, also update
    // FilterActionSetStatus::stati above
    mParameterList.append(QString());
    mParameterList.append(i18nc("msg status", "Important"));
    mParameterList.append(i18nc("msg status", "Read"));
    mParameterList.append(i18nc("msg status", "Unread"));
    mParameterList.append(i18nc("msg status", "Replied"));
    mParameterList.append(i18nc("msg status", "Forwarded"));
    mParameterList.append(i18nc("msg status", "Watched"));
    mParameterList.append(i18nc("msg status", "Ignored"));
    mParameterList.append(i18nc("msg status", "Spam"));
    mParameterList.append(i18nc("msg status", "Ham"));
    mParameterList.append(i18nc("msg status", "Action Item"));

    mParameter = mParameterList.at(0);
}

SearchRule::RequiredPart FilterActionStatus::requiredPart() const
{
    return SearchRule::Envelope;
}

bool FilterActionStatus::isEmpty() const
{
    return mParameter.isEmpty();
}

QString FilterActionStatus::realStatusString(const QString &statusStr)
{
    QString result(statusStr);

    if (result.size() == 2) {
        result.remove(QLatin1Char('U'));
    }

    return result;
}

void FilterActionStatus::argsFromString(const QString &argsStr)
{
    if (argsStr.length() == 1) {
        Akonadi::MessageStatus status;

        for (int i = 0; i < FilterActionStatus::StatiCount; ++i) {
            status = stati[i];
            if (realStatusString(status.statusStr()) == QLatin1String(argsStr.toLatin1())) {
                mParameter = mParameterList.at(i + 1);
                return;
            }
        }
    }

    mParameter = mParameterList.at(0);
}

QString FilterActionStatus::argsAsString() const
{
    const int index = mParameterList.indexOf(mParameter);
    if (index < 1) {
        return QString();
    }

    return realStatusString(FilterActionStatus::stati[index - 1].statusStr());
}

QString FilterActionStatus::displayString() const
{
    return label() + QLatin1String(" \"") + mParameter + QLatin1String("\"");
}

QString FilterActionStatus::informationAboutNotValidAction() const
{
    return i18n("Status is missing.");
}

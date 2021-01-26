/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionsendfakedisposition.h"

#include "mailcommon/mdnstateattribute.h"
#include <KLocalizedString>

using namespace MailCommon;

// if you change this list, also update
// the count in argsFromString
static const KMime::MDN::DispositionType mdns[] = {
    KMime::MDN::Displayed,
    KMime::MDN::Deleted,
    KMime::MDN::Dispatched,
    KMime::MDN::Processed,
    KMime::MDN::Denied,
    KMime::MDN::Failed,
};
static const int numMDNs = sizeof(mdns) / sizeof(*mdns);

FilterActionSendFakeDisposition::FilterActionSendFakeDisposition(QObject *parent)
    : FilterActionWithStringList(QStringLiteral("fake mdn"), i18n("Send Fake MDN"), parent)
{
    // if you change this list, also update
    // mdns above
    mParameterList.append(QString());
    mParameterList.append(i18nc("MDN type", "Ignore"));
    mParameterList.append(i18nc("MDN type", "Displayed"));
    mParameterList.append(i18nc("MDN type", "Deleted"));
    mParameterList.append(i18nc("MDN type", "Dispatched"));
    mParameterList.append(i18nc("MDN type", "Processed"));
    mParameterList.append(i18nc("MDN type", "Denied"));
    mParameterList.append(i18nc("MDN type", "Failed"));

    mParameter = mParameterList.at(0);
}

FilterAction *FilterActionSendFakeDisposition::newAction()
{
    return new FilterActionSendFakeDisposition;
}

bool FilterActionSendFakeDisposition::isEmpty() const
{
    const int index = mParameterList.indexOf(mParameter);
    return index < 1;
}

FilterAction::ReturnCode FilterActionSendFakeDisposition::process(ItemContext &context, bool) const
{
    const int index = mParameterList.indexOf(mParameter);
    if (index < 1) {
        return ErrorButGoOn;
    }

    if (index == 1) { // ignore
        if (context.item().hasAttribute<MailCommon::MDNStateAttribute>()) {
            context.item().attribute<MailCommon::MDNStateAttribute>()->setMDNState(MailCommon::MDNStateAttribute::MDNIgnore);
            context.setNeedsFlagStore();
        }
    } else { // send
        sendMDN(context.item(), mdns[index - 2]); // skip first two entries: "" and "ignore"
    }

    return GoOn;
}

SearchRule::RequiredPart FilterActionSendFakeDisposition::requiredPart() const
{
    return SearchRule::CompleteMessage;
}

void FilterActionSendFakeDisposition::argsFromString(const QString &argsStr)
{
    if (argsStr.length() == 1) {
        if (argsStr[0] == QLatin1Char('I')) { // ignore
            mParameter = mParameterList.at(1);
            return;
        }

        for (int i = 0; i < numMDNs; ++i) {
            if (char(mdns[i]) == argsStr[0]) { // send
                mParameter = mParameterList.at(i + 2);
                return;
            }
        }
    }

    mParameter = mParameterList.at(0);
}

QString FilterActionSendFakeDisposition::argsAsString() const
{
    const int index = mParameterList.indexOf(mParameter);
    if (index < 1) {
        return QString();
    }

    return QString(QChar(index < 2 ? 'I' : char(mdns[index - 2])));
}

QString FilterActionSendFakeDisposition::displayString() const
{
    return label() + QLatin1String(" \"") + mParameter + QLatin1String("\"");
}

QString FilterActionSendFakeDisposition::informationAboutNotValidAction() const
{
    return i18n("Fake type undefined.");
}

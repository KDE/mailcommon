/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 * SPDX-FileCopyrightText: 2012 Andras Mantia <amantia@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteraction.h"

#include "../kernel/mailkernel.h"
#include "filter/mdnadvicedialog.h"
#include "mailcommon_debug.h"
#include "mdn/mdnadvicehelper.h"
#include "util/mailutil.h"

#include <MessageComposer/MessageFactoryNG>
#include <MessageComposer/MessageSender>
#include <MessageComposer/Util>
#include <MessageViewer/MessageViewerSettings>

#include <KLocalizedString>

using namespace MailCommon;

FilterAction::FilterAction(const QString &name, const QString &label, QObject *parent)
    : QObject(parent)
    , mName(name)
    , mLabel(label)
{
}

FilterAction::~FilterAction()
{
}

QString FilterAction::label() const
{
    return mLabel;
}

QString FilterAction::name() const
{
    return mName;
}

bool FilterAction::isEmpty() const
{
    return false;
}

QString FilterAction::informationAboutNotValidAction() const
{
    return QString();
}

FilterAction *FilterAction::newAction()
{
    return nullptr;
}

QWidget *FilterAction::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    w->setObjectName(QStringLiteral("empty_widget"));
    return w;
}

void FilterAction::applyParamWidgetValue(QWidget *)
{
}

void FilterAction::setParamWidgetValue(QWidget *) const
{
}

void FilterAction::clearParamWidget(QWidget *) const
{
}

bool FilterAction::argsFromStringInteractive(const QString &argsStr, const QString &filterName)
{
    Q_UNUSED(filterName)
    argsFromString(argsStr);
    return false;
}

QString FilterAction::argsAsStringReal() const
{
    return argsAsString();
}

bool FilterAction::folderRemoved(const Akonadi::Collection &, const Akonadi::Collection &)
{
    return false;
}

void FilterAction::sendMDN(const Akonadi::Item &item, KMime::MDN::DispositionType type, const QVector<KMime::MDN::DispositionModifier> &modifiers)
{
    const KMime::Message::Ptr msg = MessageComposer::Util::message(item);
    if (!msg) {
        return;
    }

    const QPair<bool, KMime::MDN::SendingMode> mdnSend = MDNAdviceHelper::instance()->checkAndSetMDNInfo(item, type, true);
    if (mdnSend.first) {
        const int quote = MessageViewer::MessageViewerSettings::self()->quoteMessage();
        QString receiptTo;
        if (auto hrd = msg->headerByType("Disposition-Notification-To")) {
            receiptTo = hrd->asUnicodeString();
        }
        if (receiptTo.isEmpty()) {
            return;
        }
        MessageComposer::MessageFactoryNG factory(msg, Akonadi::Item().id());
        factory.setIdentityManager(KernelIf->identityManager());
        factory.setFolderIdentity(MailCommon::Util::folderIdentity(item));

        const KMime::Message::Ptr mdn = factory.createMDN(KMime::MDN::AutomaticAction, type, mdnSend.second, quote, modifiers);
        if (mdn) {
            if (!KernelIf->msgSender()->send(mdn, MessageComposer::MessageSender::SendLater)) {
                qCDebug(MAILCOMMON_LOG) << "Sending failed.";
            }
        }
    }
}

QStringList FilterAction::sieveRequires() const
{
    return QStringList();
}

QString FilterAction::sieveCode() const
{
    return i18n("### \"action '%1' not supported\"", name());
}

/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionsetidentity.h"
#include <MessageCore/StringUtil>

#include "filter/dialog/filteractionmissingidentitydialog.h"
#include "kernel/mailkernel.h"

#include <KIdentityManagementCore/Identity>
#include <KIdentityManagementCore/IdentityManager>
#include <KIdentityManagementWidgets/IdentityCombo>

#include <KLocalizedString>
#include <QPointer>

using namespace MailCommon;

FilterAction *FilterActionSetIdentity::newAction()
{
    return new FilterActionSetIdentity;
}

FilterActionSetIdentity::FilterActionSetIdentity(QObject *parent)
    : FilterActionWithUOID(QStringLiteral("set identity"), i18n("Set Identity To"), parent)
{
    mParameter = KernelIf->identityManager()->defaultIdentity().uoid();
}

bool FilterActionSetIdentity::argsFromStringInteractive(const QString &argsStr, const QString &filterName)
{
    bool needUpdate = false;
    argsFromString(argsStr);
    if (KernelIf->identityManager()->identityForUoid(mParameter).isNull()) {
        QPointer<MailCommon::FilterActionMissingIdentityDialog> dlg = new MailCommon::FilterActionMissingIdentityDialog(filterName);
        if (dlg->exec()) {
            mParameter = dlg->selectedIdentity();
            needUpdate = true;
        } else {
            mParameter = -1;
        }
        delete dlg;
    }
    return needUpdate;
}

FilterAction::ReturnCode FilterActionSetIdentity::process(ItemContext &context, bool applyOnOutbound) const
{
    const KIdentityManagementCore::Identity &ident = KernelIf->identityManager()->identityForUoid(mParameter);

    if (ident.isNull()) {
        return ErrorButGoOn;
    }

    const auto msg = context.item().payload<KMime::Message::Ptr>();
    uint currentId = 0;
    if (auto hrd = msg->headerByType("X-KMail-Identity")) {
        currentId = hrd->asUnicodeString().trimmed().toUInt();
    }
    if (currentId != mParameter) {
        auto header = std::unique_ptr<KMime::Headers::Generic>(new KMime::Headers::Generic("X-KMail-Identity"));
        header->fromUnicodeString(QString::number(mParameter));
        if (applyOnOutbound) {
            msg->from()->fromUnicodeString(ident.fullEmailAddr());
            if (!ident.bcc().isEmpty()) {
                const auto mailboxes = KMime::Types::Mailbox::listFromUnicodeString(ident.bcc());
                for (const KMime::Types::Mailbox &mailbox : mailboxes) {
                    msg->bcc()->addAddress(mailbox);
                }
            }
        }
        msg->setHeader(std::move(header));
        msg->assemble();

        context.setNeedsPayloadStore();
    }

    return GoOn;
}

SearchRule::RequiredPart FilterActionSetIdentity::requiredPart() const
{
    return SearchRule::CompleteMessage;
}

QWidget *FilterActionSetIdentity::createParamWidget(QWidget *parent) const
{
    auto comboBox = new KIdentityManagementWidgets::IdentityCombo(KernelIf->identityManager(), parent);
    comboBox->setObjectName(QLatin1StringView("identitycombobox"));
    comboBox->setCurrentIdentity(mParameter);

    connect(comboBox, &KIdentityManagementWidgets::IdentityCombo::currentIndexChanged, this, &FilterActionSetIdentity::filterActionModified);
    return comboBox;
}

void FilterActionSetIdentity::applyParamWidgetValue(QWidget *paramWidget)
{
    const KIdentityManagementWidgets::IdentityCombo *comboBox = qobject_cast<KIdentityManagementWidgets::IdentityCombo *>(paramWidget);
    Q_ASSERT(comboBox);

    mParameter = comboBox->currentIdentity();
}

void FilterActionSetIdentity::clearParamWidget(QWidget *paramWidget) const
{
    auto comboBox = qobject_cast<KIdentityManagementWidgets::IdentityCombo *>(paramWidget);
    Q_ASSERT(comboBox);

    comboBox->setCurrentIndex(0);
}

void FilterActionSetIdentity::setParamWidgetValue(QWidget *paramWidget) const
{
    auto comboBox = qobject_cast<KIdentityManagementWidgets::IdentityCombo *>(paramWidget);
    Q_ASSERT(comboBox);

    comboBox->setCurrentIdentity(mParameter);
}

#include "moc_filteractionsetidentity.cpp"

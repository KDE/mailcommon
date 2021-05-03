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

#include <KIdentityManagement/Identity>
#include <KIdentityManagement/IdentityCombo>
#include <KIdentityManagement/IdentityManager>

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
    const KIdentityManagement::Identity &ident = KernelIf->identityManager()->identityForUoid(mParameter);

    if (ident.isNull()) {
        return ErrorButGoOn;
    }

    const auto msg = context.item().payload<KMime::Message::Ptr>();
    uint currentId = 0;
    if (auto hrd = msg->headerByType("X-KMail-Identity")) {
        currentId = hrd->asUnicodeString().trimmed().toUInt();
    }
    if (currentId != mParameter) {
        auto header = new KMime::Headers::Generic("X-KMail-Identity");
        header->fromUnicodeString(QString::number(mParameter), "utf-8");
        if (applyOnOutbound) {
            msg->from()->fromUnicodeString(ident.fullEmailAddr(), "utf-8");
            if (!ident.bcc().isEmpty()) {
                const auto mailboxes = KMime::Types::Mailbox::listFromUnicodeString(ident.bcc());
                for (const KMime::Types::Mailbox &mailbox : mailboxes) {
                    msg->bcc()->addAddress(mailbox);
                }
            }
        }
        msg->setHeader(header);
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
    auto comboBox = new KIdentityManagement::IdentityCombo(KernelIf->identityManager(), parent);
    comboBox->setObjectName(QStringLiteral("identitycombobox"));
    comboBox->setCurrentIdentity(mParameter);

    connect(comboBox, QOverload<int>::of(&KIdentityManagement::IdentityCombo::currentIndexChanged), this, &FilterActionSetIdentity::filterActionModified);
    return comboBox;
}

void FilterActionSetIdentity::applyParamWidgetValue(QWidget *paramWidget)
{
    const KIdentityManagement::IdentityCombo *comboBox = qobject_cast<KIdentityManagement::IdentityCombo *>(paramWidget);
    Q_ASSERT(comboBox);

    mParameter = comboBox->currentIdentity();
}

void FilterActionSetIdentity::clearParamWidget(QWidget *paramWidget) const
{
    auto comboBox = qobject_cast<KIdentityManagement::IdentityCombo *>(paramWidget);
    Q_ASSERT(comboBox);

    comboBox->setCurrentIndex(0);
}

void FilterActionSetIdentity::setParamWidgetValue(QWidget *paramWidget) const
{
    auto comboBox = qobject_cast<KIdentityManagement::IdentityCombo *>(paramWidget);
    Q_ASSERT(comboBox);

    comboBox->setCurrentIdentity(mParameter);
}

/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionforward.h"
#include "mailcommon_debug.h"

#include "filter/dialog/filteractionmissingtemplatedialog.h"
#include "kernel/mailkernel.h"
#include "util/mailutil.h"

#include <Akonadi/Contact/EmailAddressRequester>
#include <MessageComposer/MessageFactoryNG>
#include <MessageComposer/MessageSender>
#include <MessageCore/StringUtil>
#include <TemplateParser/CustomTemplates>
#include <customtemplates_kfg.h>

#include <KComboBox>
#include <KLineEdit>
#include <KLocalizedString>

#include <QHBoxLayout>

using namespace MailCommon;

FilterAction *FilterActionForward::newAction()
{
    return new FilterActionForward;
}

FilterActionForward::FilterActionForward(QObject *parent)
    : FilterActionWithAddress(QStringLiteral("forward"), i18nc("Forward directly not with a command", "Forward To"), parent)
{
}

FilterAction::ReturnCode FilterActionForward::process(ItemContext &context, bool) const
{
    if (mParameter.isEmpty()) {
        return ErrorButGoOn;
    }

    const auto msg = context.item().payload<KMime::Message::Ptr>();
    // avoid endless loops when this action is used in a filter
    // which applies to sent messages
    if (MessageCore::StringUtil::addressIsInAddressList(mParameter, QStringList(msg->to()->asUnicodeString()))) {
        qCWarning(MAILCOMMON_LOG) << "Attempt to forward to recipient of original message, ignoring.";
        return ErrorButGoOn;
    }
#if 0 // PORT ME TO ASync
    MessageComposer::MessageFactoryNG factory(msg, context.item().id());
    factory.setIdentityManager(KernelIf->identityManager());
    factory.setFolderIdentity(Util::folderIdentity(context.item()));
    factory.setTemplate(mTemplate);

    KMime::Message::Ptr fwdMsg = factory.createForward();
    fwdMsg->to()->fromUnicodeString(fwdMsg->to()->asUnicodeString() + QLatin1Char(',') + mParameter, "utf-8");
    if (!KernelIf->msgSender()->send(fwdMsg, MessageComposer::MessageSender::SendDefault)) {
        qCWarning(MAILCOMMON_LOG) << "FilterAction: could not forward message (sending failed)";
        return ErrorButGoOn; // error: couldn't send
    } else {
        sendMDN(context.item(), KMime::MDN::Dispatched);
    }
#endif
    // (the msgSender takes ownership of the message, so don't delete it here)
    return GoOn;
}

SearchRule::RequiredPart FilterActionForward::requiredPart() const
{
    return SearchRule::CompleteMessage;
}

QWidget *FilterActionForward::createParamWidget(QWidget *parent) const
{
    auto addressAndTemplate = new QWidget(parent);
    auto layout = new QHBoxLayout(addressAndTemplate);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *addressEdit = FilterActionWithAddress::createParamWidget(addressAndTemplate);
    addressEdit->setObjectName(QStringLiteral("addressEdit"));
    layout->addWidget(addressEdit);

    auto addressRequester = qobject_cast<Akonadi::EmailAddressRequester *>(addressEdit);
    Q_ASSERT(addressRequester);
    KLineEdit *lineEdit = addressRequester->lineEdit();
    lineEdit->setClearButtonEnabled(true);
    lineEdit->setTrapReturnKey(true);
    lineEdit->setToolTip(i18n("The addressee to whom the message will be forwarded."));
    lineEdit->setWhatsThis(i18n("The filter will forward the message to the addressee entered here."));

    auto templateCombo = new KComboBox(addressAndTemplate);
    templateCombo->setMinimumWidth(50);
    templateCombo->setObjectName(QStringLiteral("templateCombo"));
    layout->addWidget(templateCombo);

    templateCombo->addItem(i18n("Default Template"));

    const QStringList templateNames = SettingsIf->customTemplates();
    for (const QString &templateName : templateNames) {
        TemplateParser::CTemplates templat(templateName);
        if (templat.type() == TemplateParser::CustomTemplates::TForward || templat.type() == TemplateParser::CustomTemplates::TUniversal) {
            templateCombo->addItem(templateName);
        }
    }

    templateCombo->setEnabled(templateCombo->count() > 1);
    templateCombo->setToolTip(i18n("The template used when forwarding"));
    templateCombo->setWhatsThis(i18n("Set the forwarding template that will be used with this filter."));
    connect(templateCombo, QOverload<int>::of(&KComboBox::currentIndexChanged), this, &FilterActionForward::filterActionModified);
    connect(addressRequester, &Akonadi::EmailAddressRequester::textChanged, this, &FilterActionForward::filterActionModified);

    return addressAndTemplate;
}

void FilterActionForward::applyParamWidgetValue(QWidget *paramWidget)
{
    auto *addressEdit = paramWidget->findChild<QWidget *>(QStringLiteral("addressEdit"));
    Q_ASSERT(addressEdit);
    FilterActionWithAddress::applyParamWidgetValue(addressEdit);

    const auto templateCombo = paramWidget->findChild<KComboBox *>(QStringLiteral("templateCombo"));
    Q_ASSERT(templateCombo);

    if (templateCombo->currentIndex() == 0) {
        // Default template, so don't use a custom one
        mTemplate.clear();
    } else {
        mTemplate = templateCombo->currentText();
    }
}

void FilterActionForward::setParamWidgetValue(QWidget *paramWidget) const
{
    auto *addressEdit = paramWidget->findChild<QWidget *>(QStringLiteral("addressEdit"));
    Q_ASSERT(addressEdit);
    FilterActionWithAddress::setParamWidgetValue(addressEdit);

    const auto templateCombo = paramWidget->findChild<KComboBox *>(QStringLiteral("templateCombo"));
    Q_ASSERT(templateCombo);

    if (mTemplate.isEmpty()) {
        templateCombo->setCurrentIndex(0);
    } else {
        int templateIndex = templateCombo->findText(mTemplate);
        if (templateIndex != -1) {
            templateCombo->setCurrentIndex(templateIndex);
        } else {
            mTemplate.clear();
        }
    }
}

void FilterActionForward::clearParamWidget(QWidget *paramWidget) const
{
    auto *addressEdit = paramWidget->findChild<QWidget *>(QStringLiteral("addressEdit"));
    Q_ASSERT(addressEdit);
    FilterActionWithAddress::clearParamWidget(addressEdit);

    const auto templateCombo = paramWidget->findChild<KComboBox *>(QStringLiteral("templateCombo"));
    Q_ASSERT(templateCombo);

    templateCombo->setCurrentIndex(0);
}

// We simply place a "@$$@" between the two parameters. The template is the last
// parameter in the string, for compatibility reasons.
namespace
{
inline const QString forwardFilterArgsSeperator()
{
    return QStringLiteral("@$$@");
}
}
void FilterActionForward::argsFromString(const QString &argsStr)
{
    const int seperatorPos = argsStr.indexOf(forwardFilterArgsSeperator());

    if (seperatorPos == -1) {
        // Old config, assume that the whole string is the addressee
        FilterActionWithAddress::argsFromString(argsStr);
    } else {
        const QString addressee = argsStr.left(seperatorPos);
        mTemplate = argsStr.mid(seperatorPos + forwardFilterArgsSeperator().length());
        FilterActionWithAddress::argsFromString(addressee);
    }
}

bool FilterActionForward::argsFromStringInteractive(const QString &argsStr, const QString &filterName)
{
    bool needUpdate = false;
    argsFromString(argsStr);
    if (!mTemplate.isEmpty()) {
        const QStringList templateNames = SettingsIf->customTemplates();
        QStringList currentTemplateList;
        currentTemplateList << i18n("Default Template");
        for (const QString &templateName : templateNames) {
            TemplateParser::CTemplates templat(templateName);
            if (templat.type() == TemplateParser::CustomTemplates::TForward || templat.type() == TemplateParser::CustomTemplates::TUniversal) {
                if (templateName == mTemplate) {
                    return false;
                }
                currentTemplateList << templateName;
            }
        }
        QPointer<MailCommon::FilterActionMissingTemplateDialog> dlg = new MailCommon::FilterActionMissingTemplateDialog(currentTemplateList, filterName);
        if (dlg->exec()) {
            mTemplate = dlg->selectedTemplate();
            needUpdate = true;
        }
        delete dlg;
    }
    return needUpdate;
}

QString FilterActionForward::argsAsString() const
{
    return FilterActionWithAddress::argsAsString() + forwardFilterArgsSeperator() + mTemplate;
}

QString FilterActionForward::displayString() const
{
    if (mTemplate.isEmpty()) {
        return i18n("Forward to %1 with default template", mParameter);
    } else {
        return i18n("Forward to %1 with template %2", mParameter, mTemplate);
    }
}

QString FilterActionForward::informationAboutNotValidAction() const
{
    return i18n("Email address was not defined.");
}

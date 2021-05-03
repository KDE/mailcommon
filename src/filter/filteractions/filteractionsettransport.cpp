/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionsettransport.h"

#include "filter/dialog/filteractionmissingtransportdialog.h"

#include <KLocalizedString>
#include <MailTransport/Transport>
#include <MailTransport/TransportComboBox>
#include <MailTransport/TransportManager>

#include <QPointer>

using namespace MailCommon;

FilterAction *FilterActionSetTransport::newAction()
{
    return new FilterActionSetTransport;
}

QWidget *FilterActionSetTransport::createParamWidget(QWidget *parent) const
{
    auto transportCombobox = new MailTransport::TransportComboBox(parent);
    transportCombobox->setObjectName(QStringLiteral("transportcombobox"));
    setParamWidgetValue(transportCombobox);
    connect(transportCombobox,
            QOverload<int>::of(&MailTransport::TransportComboBox::currentIndexChanged),
            this,
            &FilterActionSetTransport::filterActionModified);
    return transportCombobox;
}

FilterActionSetTransport::FilterActionSetTransport(QObject *parent)
    : FilterAction(QStringLiteral("set transport"), i18n("Set Transport To"), parent)
    , mParameter(-1)
{
}

bool FilterActionSetTransport::argsFromStringInteractive(const QString &argsStr, const QString &filterName)
{
    bool needUpdate = false;
    argsFromString(argsStr);
    if (!MailTransport::TransportManager::self()->transportById(mParameter, false)) {
        QPointer<MailCommon::FilterActionMissingTransportDialog> dlg = new MailCommon::FilterActionMissingTransportDialog(filterName);
        if (dlg->exec()) {
            mParameter = dlg->selectedTransport();
            needUpdate = true;
        } else {
            mParameter = -1;
        }
        delete dlg;
    }
    return needUpdate;
}

FilterAction::ReturnCode FilterActionSetTransport::process(ItemContext &context, bool) const
{
    if (isEmpty()) {
        return ErrorButGoOn;
    }

    const MailTransport::Transport *transport = MailTransport::TransportManager::self()->transportById(mParameter);
    // Error if we don't have transport here.
    if (!transport) {
        return ErrorButGoOn;
    }

    const auto msg = context.item().payload<KMime::Message::Ptr>();
    auto header = new KMime::Headers::Generic("X-KMail-Transport");
    header->fromUnicodeString(argsAsString(), "utf-8");
    msg->setHeader(header);
    msg->assemble();

    context.setNeedsPayloadStore();

    return GoOn;
}

SearchRule::RequiredPart FilterActionSetTransport::requiredPart() const
{
    return SearchRule::CompleteMessage;
}

void FilterActionSetTransport::applyParamWidgetValue(QWidget *paramWidget)
{
    const MailTransport::TransportComboBox *comboBox = qobject_cast<MailTransport::TransportComboBox *>(paramWidget);
    Q_ASSERT(comboBox);

    mParameter = comboBox->currentTransportId();
    mTransportName.clear();
}

void FilterActionSetTransport::clearParamWidget(QWidget *paramWidget) const
{
    auto comboBox = qobject_cast<MailTransport::TransportComboBox *>(paramWidget);
    Q_ASSERT(comboBox);

    comboBox->setCurrentIndex(0);
}

void FilterActionSetTransport::setParamWidgetValue(QWidget *paramWidget) const
{
    auto comboBox = qobject_cast<MailTransport::TransportComboBox *>(paramWidget);
    Q_ASSERT(comboBox);

    comboBox->setCurrentTransport(mParameter);
}

bool FilterActionSetTransport::isEmpty() const
{
    return mParameter == -1;
}

void FilterActionSetTransport::argsFromString(const QString &argsStr)
{
    bool ok;
    const int val = argsStr.trimmed().toInt(&ok);
    mParameter = ok ? val : -1;
}

QString FilterActionSetTransport::argsAsString() const
{
    return QString::number(mParameter);
}

QString FilterActionSetTransport::displayString() const
{
    if (mTransportName.isEmpty()) {
        const QStringList listNames = MailTransport::TransportManager::self()->transportNames();
        const int index = MailTransport::TransportManager::self()->transportIds().indexOf(mParameter);
        if (index != -1) {
            mTransportName = listNames.at(index);
        }
    }
    return label() + QLatin1String(" \"") + (mTransportName.isEmpty() ? argsAsString().toHtmlEscaped() : mTransportName) + QLatin1String("\"");
}

QString FilterActionSetTransport::informationAboutNotValidAction() const
{
    return i18n("Mail transport not defined.");
}

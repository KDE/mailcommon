/*
 * Copyright (c) 2017  Daniel Vrátil <dvratil@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "filteractionencrypt.h"
#include "mailcommon_debug.h"

#include <QEventLoop>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>

#include <KLocalizedString>

#include <KMime/Message>

#include <QGpgME/EncryptJob>
#include <QGpgME/KeyListJob>
#include <QGpgME/ListAllKeysJob>
#include <QGpgME/Protocol>
#include <gpgme++/encryptionresult.h>
#include <gpgme++/keylistresult.h>

#include <Libkleo/KeySelectionCombo>
#include <Libkleo/DefaultKeyFilter>

#include <MessageComposer/EncryptJob>

#include <Akonadi/KMime/MessageFlags>

#include <KColorScheme>

using namespace MailCommon;

#define LISTING_FINISHED "listingFinished"

FilterActionEncrypt::FilterActionEncrypt(QObject *parent)
    : FilterActionWithCrypto(QStringLiteral("encrypt"), i18n("Encrypt"), parent)
    , mKeyCache(Kleo::KeyCache::instance())
{
}

FilterActionEncrypt::~FilterActionEncrypt()
{
}

FilterAction *FilterActionEncrypt::newAction()
{
    return new FilterActionEncrypt();
}

QString FilterActionEncrypt::displayString() const
{
    return label();
}

QString FilterActionEncrypt::argsAsString() const
{
    if (mKey.isNull()) {
        return {};
    }

    const auto proto = ((mKey.protocol() == GpgME::OpenPGP) ? QStringLiteral("PGP")
                                                            : QStringLiteral("SMIME"));
    return QStringLiteral("%1:%2:%3").arg(proto, QString::number(int(mReencrypt)),
                                          QString::fromLatin1(mKey.primaryFingerprint()));
}

void FilterActionEncrypt::argsFromString(const QString &argsStr)
{
    const int pos = argsStr.indexOf(QLatin1Char(':'));
    const auto protoStr = argsStr.leftRef(pos);

    QGpgME::Protocol *proto = {};
    if (protoStr == QLatin1String("PGP")) {
        proto = QGpgME::openpgp();
    } else if (protoStr == QLatin1String("SMIME")) {
        proto = QGpgME::smime();
    } else {
        qCWarning(MAILCOMMON_LOG) << "Unknown protocol specified:" << protoStr;
        return;
    }

    mReencrypt = static_cast<bool>(argsStr.midRef(pos + 1, 1).toInt());

    const auto fp = argsStr.mid(pos + 3);
    auto listJob = proto->keyListJob(false, true, true);

    std::vector<GpgME::Key> keys;
    auto result = listJob->exec({ fp }, true, keys);
    listJob->deleteLater();

    if (result.error()) {
        qCWarning(MAILCOMMON_LOG) << "Failed to retrieve keys:" << result.error().asString();
        return;
    }

    if (keys.empty()) {
        qCWarning(MAILCOMMON_LOG) << "Could not obtain configured key: key expired or removed?";
        // TODO: Interactively ask user to re-configure the filter
        return;
    }

    mKey = keys[0];
}

SearchRule::RequiredPart FilterActionEncrypt::requiredPart() const
{
    return SearchRule::CompleteMessage;
}

FilterAction::ReturnCode FilterActionEncrypt::process(ItemContext &context, bool) const
{
    if (mKey.isNull()) {
        qCWarning(MAILCOMMON_LOG) << "FilterActionEncrypt::process called without filter having a key!";
        return ErrorButGoOn;
    }

    auto &item = context.item();
    if (!item.hasPayload<KMime::Message::Ptr>()) {
        qCWarning(MAILCOMMON_LOG) << "Item" << item.id() << "does not contain KMime::Message payload!";
        return ErrorNeedComplete;
    }

    auto msg = item.payload<KMime::Message::Ptr>();
    if (KMime::isEncrypted(msg.data())) {
        if (mReencrypt) {
            // Make sure the email is not already encrypted by the mKey - this is
            // a little expensive, but still much cheaper than modifying and
            // re-uploading the email to the server
            const auto encryptionKeys = getEncryptionKeysFromContent(msg, mKey.protocol());
            qCDebug(MAILCOMMON_LOG) << "Item" << item.id() << "encrypted by following keys: " << encryptionKeys;
            if (!encryptionKeys.isEmpty()) {
                if (mKey.protocol() == GpgME::OpenPGP) {
                    std::vector<std::string> ids;
                    ids.reserve(encryptionKeys.size());
                    for (const auto key : encryptionKeys) {
                        ids.push_back(key.toStdString());
                    }
                    for (const auto key : mKeyCache->findByKeyIDOrFingerprint(ids)) {
                        if (qstrcmp(key.primaryFingerprint(), mKey.primaryFingerprint()) == 0) {
                            // This email is already encrypted with the target key,
                            // so there's no need to re-encrypt it
                            qCDebug(MAILCOMMON_LOG) << "Item" << item.id() << "already encrypted with" << mKey.primaryFingerprint() << ", not re-encrypting";
                            return GoOn;
                        }
                    }
                } else if (mKey.protocol() == GpgME::CMS) {
                    // We are only able to get serial
                    for (const auto key : mKeyCache->secretKeys()) {
                        if (qstrcmp(key.issuerSerial(), mKey.issuerSerial()) == 0) {
                            // This email is already encrypted with the target key,
                            // so there's no need to re-encrypt it
                            qCDebug(MAILCOMMON_LOG) << "Item" << item.id() << "already encrypted with" << mKey.primaryFingerprint() << ", not re-encrypting";
                            return GoOn;
                        }
                    }
                }
            }
            bool dummy; // dummy
            const auto decrypted = decryptMessage(msg, dummy);
            if (!decrypted) {
                // We failed to decrypt the encrypted email - very likely we just don't
                // have the right key, so don't consider it an error
                return GoOn;
            } else {
                msg = decrypted;
            }
        } else {
            return GoOn;
        }
    }

    MessageComposer::EncryptJob encrypt;
    encrypt.setContent(msg.data());
    encrypt.setCryptoMessageFormat(mKey.protocol() == GpgME::OpenPGP ? Kleo::OpenPGPMIMEFormat : Kleo::SMIMEFormat);
    encrypt.setEncryptionKeys({ mKey });
    encrypt.exec();
    if (encrypt.error()) {
        qCWarning(MAILCOMMON_LOG) << "Encryption error:" << encrypt.errorString();
        return ErrorButGoOn;
    }

    KMime::Content *result = encrypt.content();
    result->assemble();

    auto nec = assembleMessage(msg, result);
    context.item().setPayload(nec);
    context.item().setFlag(Akonadi::MessageFlags::Encrypted);
    context.setNeedsPayloadStore();
    context.setNeedsFlagStore();

    delete result;

    return GoOn;
}

bool FilterActionEncrypt::isEmpty() const
{
    return mKey.isNull();
}

QString FilterActionEncrypt::informationAboutNotValidAction() const
{
    return i18n("No encryption key has been selected");
}

QWidget *FilterActionEncrypt::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto l = new QVBoxLayout;
    w->setLayout(l);

    auto combo = new Kleo::KeySelectionCombo(w);
    combo->setDefaultKey(QString::fromLatin1(mKey.primaryFingerprint()));

    std::shared_ptr<Kleo::DefaultKeyFilter> filter(new Kleo::DefaultKeyFilter);
    filter->setIsOpenPGP(Kleo::DefaultKeyFilter::DoesNotMatter);
    filter->setCanEncrypt(Kleo::DefaultKeyFilter::Set);
    filter->setHasSecret(Kleo::DefaultKeyFilter::Set);
    combo->setKeyFilter(filter);

    combo->setProperty(LISTING_FINISHED, false);
    connect(combo, &Kleo::KeySelectionCombo::keyListingFinished,
            combo, [combo] {
                combo->setProperty(LISTING_FINISHED, true);
            });
    connect(combo, &Kleo::KeySelectionCombo::currentKeyChanged,
            this, &FilterActionEncrypt::filterActionModified);
    l->addWidget(combo);

    auto chkBox = new QCheckBox(w);
    chkBox->setText(i18n("Re-encrypt encrypted emails with this key"));
    chkBox->setChecked(mReencrypt);
    l->addWidget(chkBox);

    auto lbl = new QLabel(w);
    auto palette = lbl->palette();
    palette.setColor(lbl->foregroundRole(), KColorScheme(QPalette::Normal).foreground(KColorScheme::NegativeText).color());
    lbl->setPalette(palette);
    lbl->setWordWrap(true);
    lbl->setText(i18n("<b>Warning:</b> the encrypted emails will be uploaded back to the server!"));
    lbl->setToolTip(i18n("<p>You will not be able to read the encrypted emails on any other computer "
                         "or email client unless you have your private key available there.</p>"
                         "<p>Also note that most webmail interfaces don't support encryption, so you "
                         "will not be able to read the encrypted emails there.</p>"));
    l->addWidget(lbl);

    return w;
}

void FilterActionEncrypt::setParamWidgetValue(QWidget *paramWidget) const
{
    if (auto combo = paramWidget->findChild<Kleo::KeySelectionCombo*>()) {
        combo->setDefaultKey(QString::fromLatin1(mKey.primaryFingerprint()));
        combo->setCurrentKey(QString::fromLatin1(mKey.primaryFingerprint()));
    }
    if (auto chkBox = paramWidget->findChild<QCheckBox*>()) {
        chkBox->setChecked(mReencrypt);
    }
}

void FilterActionEncrypt::applyParamWidgetValue(QWidget *paramWidget)
{
    if (auto combo = paramWidget->findChild<Kleo::KeySelectionCombo*>()) {
        // FIXME: This is super-ugly, but unfortunately the filtering code generates
        // several instances of this filter and passes the paramWidgets from one
        // instance to another to "copy" stuff in between, which in our case leads
        // to this method being called on an un-populated combobox
        if (!combo->property(LISTING_FINISHED).toBool()) {
            QEventLoop ev;
            connect(combo, &Kleo::KeySelectionCombo::keyListingFinished,
                    &ev, &QEventLoop::quit, Qt::QueuedConnection);
            ev.exec();
        }
        mKey = combo->currentKey();
    }
    if (auto chkBox = paramWidget->findChild<QCheckBox*>()) {
        mReencrypt = chkBox->isChecked();
    }
}
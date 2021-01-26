/*
 * SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTION_ENCRYPT_H_
#define MAILCOMMON_FILTERACTION_ENCRYPT_H_

#include "filteractionwithcrypto.h"

#include <gpgme++/key.h>

#include <Libkleo/KeyCache>

namespace MailCommon
{
class FilterActionEncrypt : public FilterActionWithCrypto
{
    Q_OBJECT
public:
    explicit FilterActionEncrypt(QObject *parent = nullptr);
    ~FilterActionEncrypt() override;

    static FilterAction *newAction();

    Q_REQUIRED_RESULT QString displayString() const override;

    Q_REQUIRED_RESULT QString argsAsString() const override;
    void argsFromString(const QString &argsStr) override;

    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;
    Q_REQUIRED_RESULT FilterAction::ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;

    Q_REQUIRED_RESULT bool isEmpty() const override;

    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;

    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QWidget *paramWidget) const override;
    void applyParamWidgetValue(QWidget *paramWidget) override;

    Q_REQUIRED_RESULT GpgME::Key key() const;

    Q_REQUIRED_RESULT bool reencrypt() const;

private:
    std::shared_ptr<const Kleo::KeyCache> mKeyCache;
    GpgME::Key mKey;
    bool mReencrypt = false;
};
} // namespace MailCommon

#endif

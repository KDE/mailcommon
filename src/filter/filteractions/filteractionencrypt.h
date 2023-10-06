/*
 * SPDX-FileCopyrightText: 2017 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#pragma once

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

    [[nodiscard]] QString displayString() const override;

    [[nodiscard]] QString argsAsString() const override;
    void argsFromString(const QString &argsStr) override;

    [[nodiscard]] SearchRule::RequiredPart requiredPart() const override;
    [[nodiscard]] FilterAction::ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;

    [[nodiscard]] bool isEmpty() const override;

    [[nodiscard]] QString informationAboutNotValidAction() const override;

    [[nodiscard]] QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QWidget *paramWidget) const override;
    void applyParamWidgetValue(QWidget *paramWidget) override;

    [[nodiscard]] GpgME::Key key() const;

    [[nodiscard]] bool reencrypt() const;

private:
    std::shared_ptr<const Kleo::KeyCache> mKeyCache;
    GpgME::Key mKey;
    bool mReencrypt = false;
};
} // namespace MailCommon

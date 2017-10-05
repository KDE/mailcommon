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

#ifndef MAILCOMMON_FILTERACTION_ENCRYPT_H_
#define MAILCOMMON_FILTERACTION_ENCRYPT_H_

#include "filteractionwithcrypto.h"

#include <gpgme++/key.h>

#include <Libkleo/KeyCache>

namespace MailCommon {
class FilterActionEncrypt : public FilterActionWithCrypto
{
    Q_OBJECT
public:
    explicit FilterActionEncrypt(QObject *parent = nullptr);
    ~FilterActionEncrypt() override;

    static FilterAction *newAction();

    QString displayString() const override;

    QString argsAsString() const override;
    void argsFromString(const QString &argsStr) override;

    SearchRule::RequiredPart requiredPart() const override;
    FilterAction::ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;

    bool isEmpty() const override;

    QString informationAboutNotValidAction() const override;

    QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QWidget *paramWidget) const override;
    void applyParamWidgetValue(QWidget *paramWidget) override;

    GpgME::Key key() const
    {
        return mKey;
    }

    bool reencrypt() const
    {
        return mReencrypt;
    }

private:
    std::shared_ptr<const Kleo::KeyCache> mKeyCache;
    GpgME::Key mKey;
    bool mReencrypt;
};
} // namespace MailCommon

#endif

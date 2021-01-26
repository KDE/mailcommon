/*

  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef FILTERACTIONUNSETSTATUS_H
#define FILTERACTIONUNSETSTATUS_H

#include "filteractionstatus.h"
namespace MailCommon
{
class FilterActionUnsetStatus : public FilterActionStatus
{
    Q_OBJECT
public:
    explicit FilterActionUnsetStatus(QObject *parent = nullptr);
    Q_REQUIRED_RESULT ReturnCode process(ItemContext &context, bool applyOnOutbound) const override;
    Q_REQUIRED_RESULT SearchRule::RequiredPart requiredPart() const override;

    static FilterAction *newAction();
    Q_REQUIRED_RESULT QString sieveCode() const override;
    Q_REQUIRED_RESULT QStringList sieveRequires() const override;
    Q_REQUIRED_RESULT bool isEmpty() const override;
    Q_REQUIRED_RESULT QString informationAboutNotValidAction() const override;

private:
    bool checkIsValid(int &index) const;
};
}

#endif // FILTERACTIONUNSETSTATUS_H

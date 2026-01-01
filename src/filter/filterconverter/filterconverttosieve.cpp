/*
  SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filterconverttosieve.h"
using namespace Qt::Literals::StringLiterals;

#include "filter/mailfilter.h"
#include "filterconverttosieveresultdialog.h"

#include <QPointer>

using namespace MailCommon;

FilterConvertToSieve::FilterConvertToSieve(const QList<MailFilter *> &filters)
    : mListFilters(filters)
{
}

FilterConvertToSieve::~FilterConvertToSieve() = default;

void FilterConvertToSieve::convert()
{
    QString result;
    if (!mListFilters.isEmpty()) {
        QStringList requiresModule;
        QString code;
        for (MailFilter *filter : std::as_const(mListFilters)) {
            filter->generateSieveScript(requiresModule, code);
            code += u'\n';
        }
        QString requireStr;
        for (const QString &require : std::as_const(requiresModule)) {
            requireStr += QStringLiteral("require \"%1\";").arg(require);
            requireStr += u'\n';
        }
        result = requireStr + code;
    }
    FilterConvertToSieveResultDialog dlg;
    dlg.setCode(result);
    dlg.exec();
}

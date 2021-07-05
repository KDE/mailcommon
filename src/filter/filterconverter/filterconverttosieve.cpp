/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filterconverttosieve.h"
#include "filter/mailfilter.h"
#include "filterconverttosieveresultdialog.h"

#include <QPointer>

using namespace MailCommon;

FilterConvertToSieve::FilterConvertToSieve(const QVector<MailFilter *> &filters)
    : mListFilters(filters)
{
}

FilterConvertToSieve::~FilterConvertToSieve()
{
}

void FilterConvertToSieve::convert()
{
    QString result;
    if (!mListFilters.isEmpty()) {
        QStringList requiresModule;
        QString code;
        for (MailFilter *filter : std::as_const(mListFilters)) {
            filter->generateSieveScript(requiresModule, code);
            code += QLatin1Char('\n');
        }
        QString requireStr;
        for (const QString &require : std::as_const(requiresModule)) {
            requireStr += QStringLiteral("require \"%1\";").arg(require);
            requireStr += QLatin1Char('\n');
        }
        result = requireStr + code;
    }
    QPointer<FilterConvertToSieveResultDialog> dlg = new FilterConvertToSieveResultDialog;
    dlg->setCode(result);
    dlg->exec();
    delete dlg;
}

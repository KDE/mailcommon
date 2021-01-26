/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "filterimporterabstract.h"
#include "filter/filteractions/filteractiondict.h"
#include "filter/filtermanager.h"
#include "filter/mailfilter.h"
#include "mailcommon_debug.h"

#include <QDomDocument>
#include <QFile>

using namespace MailCommon;

FilterImporterAbstract::FilterImporterAbstract(bool interactive)
    : mInteractive(interactive)
{
}

FilterImporterAbstract::~FilterImporterAbstract()
{
}

QVector<MailFilter *> FilterImporterAbstract::importFilter() const
{
    return mListMailFilter;
}

QStringList FilterImporterAbstract::emptyFilter() const
{
    return mEmptyFilter;
}

void FilterImporterAbstract::appendFilter(MailCommon::MailFilter *filter)
{
    if (!filter) {
        return;
    }

    // TODO verify number of condition
    filter->purify();
    if (!filter->isEmpty()) {
        // the filter is valid:
        mListMailFilter << filter;
    } else {
        mEmptyFilter << filter->name();
        // the filter is invalid:
        qCDebug(MAILCOMMON_LOG) << " Empty filter";
        delete filter;
    }
}

void FilterImporterAbstract::createFilterAction(MailCommon::MailFilter *filter, const QString &actionName, const QString &value)
{
    if (!actionName.isEmpty()) {
        FilterActionDesc *desc = MailCommon::FilterManager::filterActionDict()->value(actionName);
        if (desc) {
            FilterAction *fa = desc->create();
            //...create an instance...
            if (mInteractive) {
                fa->argsFromStringInteractive(value, filter->name());
            } else {
                fa->argsFromString(value);
            }

            //...check if it's empty and...
            if (!fa->isEmpty()) {
                //...append it if it's not and...
                filter->actions()->append(fa);
            } else {
                //...delete is else.
                delete fa;
            }
        }
    }
}

bool FilterImporterAbstract::loadDomElement(QDomDocument &doc, QFile *file)
{
    QString errorMsg;
    int errorRow;
    int errorCol;
    if (!doc.setContent(file, &errorMsg, &errorRow, &errorCol)) {
        qCDebug(MAILCOMMON_LOG) << "Unable to load document.Parse error in line " << errorRow << ", col " << errorCol << ": " << errorMsg;
        return false;
    }
    return true;
}

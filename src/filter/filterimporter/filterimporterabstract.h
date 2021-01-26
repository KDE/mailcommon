/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_FILTERIMPORTER_FILTERIMPORTERABSTRACT_P_H
#define MAILCOMMON_FILTERIMPORTER_FILTERIMPORTERABSTRACT_P_H

#include "mailcommon_export.h"
#include <QStringList>
#include <QVector>

class QFile;
class QDomDocument;

namespace MailCommon
{
class MailFilter;

class MAILCOMMON_EXPORT FilterImporterAbstract
{
public:
    explicit FilterImporterAbstract(bool interactive = true);
    ~FilterImporterAbstract();
    Q_REQUIRED_RESULT QVector<MailFilter *> importFilter() const;
    Q_REQUIRED_RESULT QStringList emptyFilter() const;

protected:
    void appendFilter(MailCommon::MailFilter *filter);
    void createFilterAction(MailCommon::MailFilter *filter, const QString &actionName, const QString &value);
    bool loadDomElement(QDomDocument &doc, QFile *file);

    QVector<MailFilter *> mListMailFilter;
    QStringList mEmptyFilter;

private:
    const bool mInteractive;
};
}

#endif // FILTERIMPORTERABSTRACT_H

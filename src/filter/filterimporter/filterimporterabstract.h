/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

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

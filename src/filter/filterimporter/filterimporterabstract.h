/*
  SPDX-FileCopyrightText: 2012-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "mailcommon_export.h"
#include <QList>
#include <QStringList>

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
    [[nodiscard]] QList<MailFilter *> importFilter() const;
    [[nodiscard]] QStringList emptyFilter() const;

protected:
    void appendFilter(MailCommon::MailFilter *filter);
    void createFilterAction(MailCommon::MailFilter *filter, const QString &actionName, const QString &value);
    bool loadDomElement(QDomDocument &doc, QFile *file);

    QList<MailFilter *> mListMailFilter;
    QStringList mEmptyFilter;

private:
    const bool mInteractive;
};
}

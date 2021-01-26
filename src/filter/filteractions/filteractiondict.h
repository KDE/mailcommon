/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef MAILCOMMON_FILTERACTIONDICT_H
#define MAILCOMMON_FILTERACTIONDICT_H

#include "mailcommon_export.h"

#include <QMultiHash>
#include <QString>
#include <QVector>
namespace MailCommon
{
class FilterAction;

using FilterActionNewFunc = FilterAction *(*)();

/**
 * @short Auxiliary struct for FilterActionDict.
 */
struct FilterActionDesc {
    QString label, name;
    FilterActionNewFunc create;
};

/**
 * @short List of known FilterAction-types.
 *
 * Dictionary that contains a list of all registered filter actions
 * with their creation functions. They are hard-coded into the
 * constructor. If you want to add a new FilterAction, make
 * sure you add the details of it in init, too.
 *
 * You will be able to find a description of a FilterAction by
 * looking up either it's (english) name or it's (i18n) label:
 * <pre>
 * FilterActionDict dict;
 * // get name of the action with label "move into folder":
 * dict[i18n("move into folder")]->name; // == "transfer"
 * // create one such action:
 * FilterAction *action = dict["transfer"]->create();
 * </pre>
 *
 * You can iterate over all known filter actions by using list.
 *
 * @author Marc Mutz <mutz@kde.org>, based on work by Stefan Taferner <taferner@kde.org>
 * @see FilterAction FilterActionDesc Filter
 */
class FilterActionDict : public QMultiHash<QString, FilterActionDesc *>
{
public:
    /**
     * Creates the filter action dictionary.
     */
    MAILCOMMON_EXPORT FilterActionDict();

    /**
     * Destroys the filter action dictionary.
     */
    MAILCOMMON_EXPORT virtual ~FilterActionDict();

    /**
     * Overloaded member function, provided for convenience. Thin
     * wrapper around QDict::insert and QPtrList::insert.
     * Inserts the resulting FilterActionDesc
     * thrice: First with the name, then with the label as key into the
     * QDict, then into the QPtrList. For that, it creates an
     * instance of the action internally and deletes it again after
     * querying it for name and label.
     */
    MAILCOMMON_EXPORT void insert(FilterActionNewFunc aNewFunc);

    /**
     * Provides read-only access to a list of all known filter
     * actions.
     */
    MAILCOMMON_EXPORT const QVector<FilterActionDesc *> &list() const;

protected:
    /**
     * Populate the dictionary with all known  FilterAction
     * types. Called automatically from the constructor.
     */
    virtual void init();

private:
    QVector<FilterActionDesc *> mList;
};
}

#endif

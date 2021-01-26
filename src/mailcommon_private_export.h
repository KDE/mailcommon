/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2007 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MAILCOMMONPRIVATE_EXPORT_H
#define MAILCOMMONPRIVATE_EXPORT_H

#include "mailcommon_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
#ifndef MAILCOMMON_TESTS_EXPORT
#define MAILCOMMON_TESTS_EXPORT MAILCOMMON_EXPORT
#endif
#else /* not compiling tests */
#define MAILCOMMON_TESTS_EXPORT
#endif

#endif

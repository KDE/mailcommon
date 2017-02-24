/*  This file is part of the KDE project
    Copyright (C) 2007 David Faure <faure@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef MAILCOMMONPRIVATE_EXPORT_H
#define MAILCOMMONPRIVATE_EXPORT_H

#include "mailcommon_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
# ifndef MAILCOMMON_TESTS_EXPORT
#  define MAILCOMMON_TESTS_EXPORT MAILCOMMON_EXPORT
# endif
#else /* not compiling tests */
# define MAILCOMMON_TESTS_EXPORT
#endif

#endif

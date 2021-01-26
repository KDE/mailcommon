/*
    SPDX-FileCopyrightText: 2015 Sandro Knauß <bugs@sandroknauss.de>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef FILTERACTIONPIPETHROUGH_TEST_H
#define FILTERACTIONPIPETHROUGH_TEST_H

#include <QObject>

namespace MailCommon
{
class FilterAction;
}

class FilterActionPipeThroughTest : public QObject
{
    Q_OBJECT
public:
    FilterActionPipeThroughTest();
private Q_SLOTS:
    void testWithNoCommand();
    void testWithInvalidCommandPath();
    void testCommandWithoutOutput();
    void testWithMailOutput();
    void testCopyMail();
    void testXUidChange();
    void testXUidUnchange();
    void testXUidRemoved();
    void shouldRequiresPart();

private:
    void setOutput(MailCommon::FilterAction *filter, const QByteArray &output);
};

#endif

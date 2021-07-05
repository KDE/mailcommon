/*
  SPDX-FileCopyrightText: 2003 Andreas Gungl <a.gungl@gmx.de>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filterlog.h"

#include "mailcommon_debug.h"
#include "messagecomposer/util.h"

#include <QFile>
#include <QTime>

#include <sys/stat.h>

using namespace MailCommon;

class Q_DECL_HIDDEN FilterLog::Private
{
public:
    Private(FilterLog *qq)
        : q(qq)
        , mMaxLogSize(512 * 1024)
        , mAllowedTypes(FilterLog::Meta | FilterLog::PatternDescription | FilterLog::RuleResult | FilterLog::PatternResult | FilterLog::AppliedAction)
    {
    }

    static FilterLog *mSelf;

    FilterLog *const q;
    QStringList mLogEntries;
    long mMaxLogSize;
    long mCurrentLogSize = 0;
    int mAllowedTypes;
    bool mLogging = false;

    void checkLogSize();
};

void FilterLog::Private::checkLogSize()
{
    if (mCurrentLogSize > mMaxLogSize && mMaxLogSize > -1) {
        qCDebug(MAILCOMMON_LOG) << "Filter log: memory limit reached, starting to discard old items, size =" << QString::number(mCurrentLogSize);

        // avoid some kind of hysteresis, shrink the log to 90% of its maximum
        while (mCurrentLogSize > (mMaxLogSize * 0.9)) {
            QStringList::Iterator it = mLogEntries.begin();
            if (it != mLogEntries.end()) {
                mCurrentLogSize -= (*it).length();
                mLogEntries.erase(it);
                qCDebug(MAILCOMMON_LOG) << "Filter log: new size =" << QString::number(mCurrentLogSize);
            } else {
                qCDebug(MAILCOMMON_LOG) << "Filter log: size reduction disaster!";
                q->clear();
            }
        }

        Q_EMIT q->logShrinked();
    }
}

FilterLog *FilterLog::Private::mSelf = nullptr;

FilterLog::FilterLog()
    : d(new Private(this))
{
}

FilterLog::~FilterLog()
{
    delete d;
}

FilterLog *FilterLog::instance()
{
    if (!FilterLog::Private::mSelf) {
        FilterLog::Private::mSelf = new FilterLog();
    }

    return FilterLog::Private::mSelf;
}

bool FilterLog::isLogging() const
{
    return d->mLogging;
}

void FilterLog::setLogging(bool active)
{
    d->mLogging = active;
    Q_EMIT logStateChanged();
}

void FilterLog::setMaxLogSize(long size)
{
    if (size < -1) {
        size = -1;
    }

    // do not allow less than 1 KByte except unlimited (-1)
    if (size >= 0 && size < 1024) {
        size = 1024;
    }

    d->mMaxLogSize = size;
    Q_EMIT logStateChanged();
    d->checkLogSize();
}

long FilterLog::maxLogSize() const
{
    return d->mMaxLogSize;
}

void FilterLog::setContentTypeEnabled(ContentType contentType, bool enable)
{
    if (enable) {
        d->mAllowedTypes |= contentType;
    } else {
        d->mAllowedTypes &= ~contentType;
    }

    Q_EMIT logStateChanged();
}

bool FilterLog::isContentTypeEnabled(ContentType contentType) const
{
    return d->mAllowedTypes & contentType;
}

void FilterLog::add(const QString &logEntry, ContentType contentType)
{
    if (isLogging() && (d->mAllowedTypes & contentType)) {
        QString timedLog;
        if (contentType & ~Meta) {
            timedLog = QLatin1Char('[') + QTime::currentTime().toString() + QLatin1String("] ") + logEntry;
        } else {
            timedLog = logEntry;
        }

        d->mLogEntries.append(timedLog);
        Q_EMIT logEntryAdded(timedLog);
        d->mCurrentLogSize += timedLog.length();
        d->checkLogSize();
    }
}

void FilterLog::addSeparator()
{
    add(QStringLiteral("------------------------------"), Meta);
}

void FilterLog::clear()
{
    d->mLogEntries.clear();
    d->mCurrentLogSize = 0;
}

QStringList FilterLog::logEntries() const
{
    return d->mLogEntries;
}

void FilterLog::dump()
{
    qCDebug(MAILCOMMON_LOG) << "----- starting filter log -----";
    for (const QString &entry : std::as_const(d->mLogEntries)) {
        qCDebug(MAILCOMMON_LOG) << entry;
    }
    qCDebug(MAILCOMMON_LOG) << "------ end of filter log ------";
}

bool FilterLog::saveToFile(const QString &fileName) const
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write("<html>\n<body>\n");
    file.write("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");
    for (const QString &entry : std::as_const(d->mLogEntries)) {
        const QString line = QLatin1String("<p>") + entry + QLatin1String("</p>") + QLatin1Char('\n');
        file.write(line.toLocal8Bit());
    }
    file.write("</body>\n</html>\n");
    file.close();
    return true;
}

QString FilterLog::recode(const QString &plain)
{
    return plain.toHtmlEscaped();
}

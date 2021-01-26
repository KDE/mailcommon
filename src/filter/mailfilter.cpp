/*
 * kmail: KDE mail client
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 * SPDX-FileCopyrightText: 2012 Andras Mantia <amantia@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

// my header
#include "mailfilter.h"

// other kmail headers
#include "dialog/filteractionmissingaccountdialog.h"
#include "filteractions/filteraction.h"
#include "filteractions/filteractiondict.h"
#include "filterlog.h"
#include "filtermanager.h"
using MailCommon::FilterLog;

#include <PimCommon/PimUtil>

// KDEPIMLIBS headers
#include <AgentManager>

// other KDE headers
#include <KConfig>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KMessageBox>
#include <QPointer>
#include <krandom.h>

#include <algorithm>
#include <boost/bind.hpp>

using namespace MailCommon;

MailFilter::MailFilter()
{
    generateRandomIdentifier();
    bApplyOnInbound = true;
    bApplyBeforeOutbound = false;
    bApplyOnOutbound = false;
    bApplyOnExplicit = true;
    bApplyOnAllFolders = false;
    bStopProcessingHere = true;
    bConfigureShortcut = false;
    bConfigureToolbar = false;
    bAutoNaming = true;
    mApplicability = All;
    bEnabled = true;
}

MailFilter::MailFilter(const KConfigGroup &aConfig, bool interactive, bool &needUpdate)
{
    needUpdate = readConfig(aConfig, interactive);
}

MailFilter::MailFilter(const MailFilter &aFilter)
{
    mIdentifier = aFilter.mIdentifier;
    mPattern = aFilter.mPattern;

    bApplyOnInbound = aFilter.applyOnInbound();
    bApplyBeforeOutbound = aFilter.applyBeforeOutbound();
    bApplyOnOutbound = aFilter.applyOnOutbound();
    bApplyOnExplicit = aFilter.applyOnExplicit();
    bApplyOnAllFolders = aFilter.applyOnAllFoldersInbound();
    bStopProcessingHere = aFilter.stopProcessingHere();
    bConfigureShortcut = aFilter.configureShortcut();
    bConfigureToolbar = aFilter.configureToolbar();
    mToolbarName = aFilter.toolbarName();
    mApplicability = aFilter.applicability();
    bAutoNaming = aFilter.isAutoNaming();
    bEnabled = aFilter.isEnabled();
    mIcon = aFilter.icon();
    mShortcut = aFilter.shortcut();

    QVectorIterator<FilterAction *> it(aFilter.mActions);
    while (it.hasNext()) {
        FilterAction *action = it.next();
        FilterActionDesc *desc = FilterManager::filterActionDict()->value(action->name());
        if (desc) {
            FilterAction *f = desc->create();
            if (f) {
                f->argsFromString(action->argsAsString());
                mActions.append(f);
            }
        }
    }

    mAccounts.clear();
    QStringList::ConstIterator it2;
    QStringList::ConstIterator end2 = aFilter.mAccounts.constEnd();
    for (it2 = aFilter.mAccounts.constBegin(); it2 != end2; ++it2) {
        mAccounts.append(*it2);
    }
}

MailFilter::~MailFilter()
{
    qDeleteAll(mActions);
}

int MailFilter::filterActionsMaximumSize()
{
    return 8;
}

void MailFilter::generateRandomIdentifier()
{
    mIdentifier = KRandom::randomString(16);
}

QString MailFilter::identifier() const
{
    return mIdentifier;
}

QString MailFilter::name() const
{
    return mPattern.name();
}

MailFilter::ReturnCode MailFilter::execActions(ItemContext &context, bool &stopIt, bool applyOnOutbound) const
{
    QVector<FilterAction *>::const_iterator it(mActions.constBegin());
    QVector<FilterAction *>::const_iterator end(mActions.constEnd());
    for (; it != end; ++it) {
        if (FilterLog::instance()->isLogging()) {
            const QString logText(i18n("<b>Applying filter action:</b> %1", (*it)->displayString()));
            FilterLog::instance()->add(logText, FilterLog::AppliedAction);
        }

        const FilterAction::ReturnCode result = (*it)->process(context, applyOnOutbound);

        switch (result) {
        case FilterAction::CriticalError:
            if (FilterLog::instance()->isLogging()) {
                const QString logText = QStringLiteral("<font color=#FF0000>%1</font>").arg(i18n("A critical error occurred. Processing stops here."));
                FilterLog::instance()->add(logText, FilterLog::AppliedAction);
            }
            // in case it's a critical error: return immediately!
            return CriticalError;
        case FilterAction::ErrorButGoOn:
            if (FilterLog::instance()->isLogging()) {
                const QString logText = QStringLiteral("<font color=#FF0000>%1</font>").arg(i18n("A problem was found while applying this action."));
                FilterLog::instance()->add(logText, FilterLog::AppliedAction);
            }
        case FilterAction::GoOn:
        case FilterAction::ErrorNeedComplete:
            break;
        }
    }

    stopIt = stopProcessingHere();

    return GoOn;
}

QVector<FilterAction *> *MailFilter::actions()
{
    return &mActions;
}

const QVector<FilterAction *> *MailFilter::actions() const
{
    return &mActions;
}

SearchPattern *MailFilter::pattern()
{
    return &mPattern;
}

const SearchPattern *MailFilter::pattern() const
{
    return &mPattern;
}

void MailFilter::setApplyOnOutbound(bool aApply)
{
    bApplyOnOutbound = aApply;
}

void MailFilter::setApplyBeforeOutbound(bool aApply)
{
    bApplyBeforeOutbound = aApply;
}

bool MailFilter::applyOnOutbound() const
{
    return bApplyOnOutbound;
}

bool MailFilter::applyBeforeOutbound() const
{
    return bApplyBeforeOutbound;
}

void MailFilter::setApplyOnInbound(bool aApply)
{
    bApplyOnInbound = aApply;
}

bool MailFilter::applyOnInbound() const
{
    return bApplyOnInbound;
}

void MailFilter::setApplyOnExplicit(bool aApply)
{
    bApplyOnExplicit = aApply;
}

bool MailFilter::applyOnExplicit() const
{
    return bApplyOnExplicit;
}

void MailFilter::setApplyOnAllFoldersInbound(bool aApply)
{
    bApplyOnAllFolders = aApply;
}

bool MailFilter::applyOnAllFoldersInbound() const
{
    return bApplyOnAllFolders;
}

void MailFilter::setApplicability(AccountType aApply)
{
    mApplicability = aApply;
}

MailFilter::AccountType MailFilter::applicability() const
{
    return mApplicability;
}

SearchRule::RequiredPart MailFilter::requiredPart(const QString &id) const
{
    // find the required message part needed for the filter
    // this can be either only the Envelope, all Header or the CompleteMessage
    // Makes the assumption that  Envelope < Header < CompleteMessage
    int requiredPart = SearchRule::Envelope;

    if (!bEnabled || !applyOnAccount(id)) {
        return static_cast<SearchRule::RequiredPart>(requiredPart);
    }

    if (pattern()) {
        requiredPart = qMax(requiredPart, static_cast<int>(pattern()->requiredPart())); // no pattern means always matches?
    }

    int requiredPartByActions = SearchRule::Envelope;

    QVector<FilterAction *> actionList = *actions();
    if (!actionList.isEmpty()) {
        requiredPartByActions =
            (*std::max_element(actionList.constBegin(),
                               actionList.constEnd(),
                               boost::bind(&MailCommon::FilterAction::requiredPart, _1) < boost::bind(&MailCommon::FilterAction::requiredPart, _2)))
                ->requiredPart();
    }
    requiredPart = qMax(requiredPart, requiredPartByActions);

    return static_cast<SearchRule::RequiredPart>(requiredPart);
}

void MailFilter::agentRemoved(const QString &identifier)
{
    mAccounts.removeAll(identifier);
}

void MailFilter::folderRemoved(const Akonadi::Collection &aFolder, const Akonadi::Collection &aNewFolder)
{
    QVectorIterator<FilterAction *> it(mActions);
    while (it.hasNext()) {
        it.next()->folderRemoved(aFolder, aNewFolder);
    }
}

void MailFilter::clearApplyOnAccount()
{
    mAccounts.clear();
}

void MailFilter::setApplyOnAccount(const QString &id, bool aApply)
{
    if (aApply && !mAccounts.contains(id)) {
        mAccounts.append(id);
    } else if (!aApply && mAccounts.contains(id)) {
        mAccounts.removeAll(id);
    }
}

bool MailFilter::applyOnAccount(const QString &id) const
{
    if (applicability() == All) {
        return true;
    }
    if (applicability() == ButImap) {
        Akonadi::AgentInstance instance = Akonadi::AgentManager::self()->instance(id);
        if (instance.isValid()) {
            return !PimCommon::Util::isImapResource(instance.type().identifier());
        } else {
            return false;
        }
    }
    if (applicability() == Checked) {
        return mAccounts.contains(id);
    }

    return false;
}

void MailFilter::setStopProcessingHere(bool aStop)
{
    bStopProcessingHere = aStop;
}

bool MailFilter::stopProcessingHere() const
{
    return bStopProcessingHere;
}

void MailFilter::setConfigureShortcut(bool aShort)
{
    bConfigureShortcut = aShort;
    bConfigureToolbar = (bConfigureToolbar && bConfigureShortcut);
}

bool MailFilter::configureShortcut() const
{
    return bConfigureShortcut;
}

void MailFilter::setConfigureToolbar(bool aTool)
{
    bConfigureToolbar = (aTool && bConfigureShortcut);
}

bool MailFilter::configureToolbar() const
{
    return bConfigureToolbar;
}

void MailFilter::setToolbarName(const QString &toolbarName)
{
    mToolbarName = toolbarName;
}

void MailFilter::setShortcut(const QKeySequence &shortcut)
{
    mShortcut = shortcut;
}

const QKeySequence &MailFilter::shortcut() const
{
    return mShortcut;
}

void MailFilter::setIcon(const QString &icon)
{
    mIcon = icon;
}

QString MailFilter::icon() const
{
    return mIcon;
}

void MailFilter::setAutoNaming(bool useAutomaticNames)
{
    bAutoNaming = useAutomaticNames;
}

bool MailFilter::isAutoNaming() const
{
    return bAutoNaming;
}

//-----------------------------------------------------------------------------
bool MailFilter::readConfig(const KConfigGroup &config, bool interactive)
{
    bool needUpdate = false;
    // MKSearchPattern::readConfig ensures
    // that the pattern is purified.
    mPattern.readConfig(config);
    mIdentifier = config.readEntry("identifier", KRandom::randomString(16));

    const QStringList sets = config.readEntry("apply-on", QStringList());
    if (sets.isEmpty() && !config.hasKey("apply-on")) {
        bApplyBeforeOutbound = false;
        bApplyOnOutbound = false;
        bApplyOnInbound = true;
        bApplyOnExplicit = true;
        bApplyOnAllFolders = false;
        mApplicability = ButImap;
    } else {
        bApplyBeforeOutbound = bool(sets.contains(QLatin1String("before-send-mail")));
        bApplyOnInbound = bool(sets.contains(QLatin1String("check-mail")));
        bApplyOnOutbound = bool(sets.contains(QLatin1String("send-mail")));
        bApplyOnExplicit = bool(sets.contains(QLatin1String("manual-filtering")));
        bApplyOnAllFolders = bool(sets.contains(QLatin1String("all-folders")));
        mApplicability = static_cast<AccountType>(config.readEntry("Applicability", static_cast<int>(ButImap)));
    }

    bStopProcessingHere = config.readEntry("StopProcessingHere", true);
    bConfigureShortcut = config.readEntry("ConfigureShortcut", false);
    QString shortcut(config.readEntry("Shortcut", QString()));
    if (!shortcut.isEmpty()) {
        QKeySequence sc(shortcut);
        setShortcut(sc);
    }
    bConfigureToolbar = config.readEntry("ConfigureToolbar", false);
    bConfigureToolbar = bConfigureToolbar && bConfigureShortcut;
    mToolbarName = config.readEntry("ToolbarName", name());
    mIcon = config.readEntry("Icon", "system-run");
    bAutoNaming = config.readEntry("AutomaticName", false);
    bEnabled = config.readEntry("Enabled", true);

    mActions.clear();

    int numActions = config.readEntry("actions", 0);
    if (numActions > filterActionsMaximumSize()) {
        numActions = filterActionsMaximumSize();
        KMessageBox::information(nullptr, i18n("<qt>Too many filter actions in filter rule <b>%1</b>.</qt>", mPattern.name()));
    }

    for (int i = 0; i < numActions; ++i) {
        const QString actName = QStringLiteral("action-name-%1").arg(i);
        const QString argsName = QStringLiteral("action-args-%1").arg(i);
        // get the action description...
        FilterActionDesc *desc = FilterManager::filterActionDict()->value(config.readEntry(actName, QString()));
        if (desc) {
            //...create an instance...
            FilterAction *fa = desc->create();
            if (fa) {
                //...load it with it's parameter...
                if (interactive) {
                    const bool ret = fa->argsFromStringInteractive(config.readEntry(argsName, QString()), name());
                    if (ret) {
                        needUpdate = true;
                    }
                } else {
                    fa->argsFromString(config.readEntry(argsName, QString()));
                }
                //...check if it's empty and...
                if (!fa->isEmpty()) {
                    //...append it if it's not and...
                    mActions.append(fa);
                } else {
                    //...delete is else.
                    delete fa;
                }
            }
        } else {
            KMessageBox::information(nullptr /* app-global modal dialog box */,
                                     i18n("<qt>Unknown filter action <b>%1</b><br />in filter rule <b>%2</b>.<br />Ignoring it.</qt>",
                                          config.readEntry(actName, QString()),
                                          mPattern.name()));
        }
    }

    mAccounts = config.readEntry("accounts-set", QStringList());
    if (!mAccounts.isEmpty() && interactive) {
        if (!MailCommon::FilterActionMissingAccountDialog::allAccountExist(mAccounts)) {
            QPointer<MailCommon::FilterActionMissingAccountDialog> dlg = new MailCommon::FilterActionMissingAccountDialog(mAccounts, name());
            if (dlg->exec()) {
                mAccounts = dlg->selectedAccount();
                needUpdate = true;
            }
            delete dlg;
        }
    }
    return needUpdate;
}

void MailFilter::generateSieveScript(QStringList &requiresModules, QString &code)
{
    mPattern.generateSieveScript(requiresModules, code);

    QVector<FilterAction *>::const_iterator it;
    QVector<FilterAction *>::const_iterator end(mActions.constEnd());

    const QString indentationStr{QStringLiteral("    ")};
    code += QLatin1String(")\n{\n");
    bool firstAction = true;
    for (it = mActions.constBegin(); it != end; ++it) {
        // Add endline here.
        if (firstAction) {
            firstAction = false;
        } else {
            code += QLatin1Char('\n');
        }
        code += indentationStr + (*it)->sieveCode();
        const QStringList lstRequires = (*it)->sieveRequires();
        for (const QString &str : lstRequires) {
            if (!requiresModules.contains(str)) {
                requiresModules.append(str);
            }
        }
    }
    if (bStopProcessingHere) {
        code += QLatin1Char('\n') + indentationStr + QStringLiteral("stop;");
    }
    code += QLatin1String("\n}\n");
}

void MailFilter::writeConfig(KConfigGroup &config, bool exportFilter) const
{
    mPattern.writeConfig(config);
    config.writeEntry("identifier", mIdentifier);

    QStringList sets;
    if (bApplyOnInbound) {
        sets.append(QStringLiteral("check-mail"));
    }
    if (bApplyBeforeOutbound) {
        sets.append(QStringLiteral("before-send-mail"));
    }
    if (bApplyOnOutbound) {
        sets.append(QStringLiteral("send-mail"));
    }
    if (bApplyOnExplicit) {
        sets.append(QStringLiteral("manual-filtering"));
    }
    if (bApplyOnAllFolders) {
        sets.append(QStringLiteral("all-folders"));
    }
    config.writeEntry("apply-on", sets);

    config.writeEntry("StopProcessingHere", bStopProcessingHere);
    config.writeEntry("ConfigureShortcut", bConfigureShortcut);
    if (!mShortcut.isEmpty()) {
        config.writeEntry("Shortcut", mShortcut.toString());
    }
    config.writeEntry("ConfigureToolbar", bConfigureToolbar);
    config.writeEntry("ToolbarName", mToolbarName);
    if (!mIcon.isEmpty()) {
        config.writeEntry("Icon", mIcon);
    }
    config.writeEntry("AutomaticName", bAutoNaming);
    config.writeEntry("Applicability", static_cast<int>(mApplicability));
    config.writeEntry("Enabled", bEnabled);
    int i;

    QVector<FilterAction *>::const_iterator it;
    QVector<FilterAction *>::const_iterator end(mActions.constEnd());

    for (i = 0, it = mActions.constBegin(); it != end; ++it, ++i) {
        config.writeEntry(QStringLiteral("action-name-%1").arg(i), (*it)->name());
        config.writeEntry(QStringLiteral("action-args-%1").arg(i), exportFilter ? (*it)->argsAsStringReal() : (*it)->argsAsString());
    }
    config.writeEntry("actions", i);
    if (!mAccounts.isEmpty()) {
        config.writeEntry("accounts-set", mAccounts);
    }
}

QString MailFilter::purify(bool removeAction)
{
    QString informationAboutNotValidAction = mPattern.purify(removeAction);

    if (mActions.isEmpty()) {
        if (!informationAboutNotValidAction.isEmpty()) {
            informationAboutNotValidAction += QLatin1Char('\n');
        }
        informationAboutNotValidAction += i18n("Any action defined.");
    } else {
        QVectorIterator<FilterAction *> it(mActions);
        it.toBack();
        while (it.hasPrevious()) {
            FilterAction *action = it.previous();
            if (action->isEmpty()) {
                if (!informationAboutNotValidAction.isEmpty()) {
                    informationAboutNotValidAction += QLatin1Char('\n');
                }
                informationAboutNotValidAction += action->informationAboutNotValidAction();
                if (removeAction) {
                    mActions.removeAll(action);
                }
            }
        }
    }

    if (!Akonadi::AgentManager::self()->instances().isEmpty()) { // safety test to ensure that Akonadi system is ready
        // Remove invalid accounts from mAccounts - just to be tidy
        QStringList::Iterator it2 = mAccounts.begin();
        while (it2 != mAccounts.end()) {
            if (!Akonadi::AgentManager::self()->instance(*it2).isValid()) {
                it2 = mAccounts.erase(it2);
            } else {
                ++it2;
            }
        }
    }
    return informationAboutNotValidAction;
}

bool MailFilter::isEmpty() const
{
    return (mPattern.isEmpty() && mActions.isEmpty()) || ((applicability() == Checked) && (bApplyOnInbound && mAccounts.isEmpty()));
}

QString MailFilter::toolbarName() const
{
    if (mToolbarName.isEmpty()) {
        return name();
    } else {
        return mToolbarName;
    }
}

const QString MailFilter::asString() const
{
    QString result;

    result += QLatin1String("Filter name: ") + name() + QLatin1String(" (") + mIdentifier + QLatin1String(")\n");
    result += mPattern.asString() + QLatin1Char('\n');

    result += QStringLiteral("Filter is %1\n").arg(bEnabled ? QStringLiteral("enabled") : QStringLiteral("disabled"));

    QVector<FilterAction *>::const_iterator it(mActions.constBegin());
    QVector<FilterAction *>::const_iterator end(mActions.constEnd());
    for (; it != end; ++it) {
        result += QStringLiteral("    action: ");
        result += (*it)->label();
        result += QLatin1Char(' ');
        result += (*it)->argsAsString();
        result += QLatin1Char('\n');
    }
    result += QStringLiteral("This filter belongs to the following sets:");
    if (bApplyOnInbound) {
        result += QStringLiteral(" Inbound");
    }
    if (bApplyBeforeOutbound) {
        result += QStringLiteral(" before-Outbound");
    }
    if (bApplyOnOutbound) {
        result += QStringLiteral(" Outbound");
    }
    if (bApplyOnExplicit) {
        result += QStringLiteral(" Explicit");
    }
    if (bApplyOnAllFolders) {
        result += QStringLiteral(" All Folders");
    }
    result += QLatin1Char('\n');
    if (bApplyOnInbound && mApplicability == All) {
        result += QStringLiteral("This filter applies to all accounts.\n");
    } else if (bApplyOnInbound && mApplicability == ButImap) {
        result += QStringLiteral("This filter applies to all but IMAP accounts.\n");
    } else if (bApplyOnInbound) {
        result += QStringLiteral("This filter applies to the following accounts:");
        if (mAccounts.isEmpty()) {
            result += QStringLiteral(" None");
        } else {
            for (QStringList::ConstIterator it2 = mAccounts.begin(), it2End = mAccounts.end(); it2 != it2End; ++it2) {
                if (Akonadi::AgentManager::self()->instance(*it2).isValid()) {
                    result += QLatin1Char(' ') + Akonadi::AgentManager::self()->instance(*it2).name();
                }
            }
        }
        result += QLatin1Char('\n');
    }
    if (bStopProcessingHere) {
        result += QStringLiteral("If it matches, processing stops at this filter.\n");
    }

    return result;
}

QDataStream &MailCommon::operator<<(QDataStream &stream, const MailCommon::MailFilter &filter)
{
    stream << filter.mIdentifier;
    stream << filter.mPattern.serialize();

    stream << filter.mActions.count();
    QVectorIterator<FilterAction *> it(filter.mActions);
    while (it.hasNext()) {
        const FilterAction *action = it.next();
        stream << action->name();
        stream << action->argsAsString();
    }

    stream << filter.mAccounts;
    stream << filter.mIcon;
    stream << filter.mToolbarName;
    stream << filter.mShortcut;
    stream << filter.bApplyOnInbound;
    stream << filter.bApplyBeforeOutbound;
    stream << filter.bApplyOnOutbound;
    stream << filter.bApplyOnExplicit;
    stream << filter.bApplyOnAllFolders;
    stream << filter.bStopProcessingHere;
    stream << filter.bConfigureShortcut;
    stream << filter.bConfigureToolbar;
    stream << filter.bAutoNaming;
    stream << filter.mApplicability;
    stream << filter.bEnabled;

    return stream;
}

QDataStream &MailCommon::operator>>(QDataStream &stream, MailCommon::MailFilter &filter)
{
    QByteArray pattern;
    int numberOfActions;
    QKeySequence shortcut;
    bool bApplyOnInbound;
    bool bApplyBeforeOutbound;
    bool bApplyOnOutbound;
    bool bApplyOnExplicit;
    bool bApplyOnAllFolders;
    bool bStopProcessingHere;
    bool bConfigureShortcut;
    bool bConfigureToolbar;
    bool bAutoNaming;
    int applicability;
    bool bEnabled;

    stream >> filter.mIdentifier;
    stream >> pattern;

    stream >> numberOfActions;
    qDeleteAll(filter.mActions);
    filter.mActions.clear();

    for (int i = 0; i < numberOfActions; ++i) {
        QString actionName;
        QString actionArguments;

        stream >> actionName;
        stream >> actionArguments;

        FilterActionDesc *description = FilterManager::filterActionDict()->value(actionName);
        if (description) {
            FilterAction *filterAction = description->create();
            if (filterAction) {
                filterAction->argsFromString(actionArguments);
                filter.mActions.append(filterAction);
            }
        }
    }

    stream >> filter.mAccounts;
    stream >> filter.mIcon;
    stream >> filter.mToolbarName;
    stream >> shortcut;
    stream >> bApplyOnInbound;
    stream >> bApplyBeforeOutbound;
    stream >> bApplyOnOutbound;
    stream >> bApplyOnExplicit;
    stream >> bApplyOnAllFolders;
    stream >> bStopProcessingHere;
    stream >> bConfigureShortcut;
    stream >> bConfigureToolbar;
    stream >> bAutoNaming;
    stream >> applicability;
    stream >> bEnabled;

    filter.mPattern.deserialize(pattern);
    filter.mShortcut = shortcut;
    filter.bApplyOnInbound = bApplyOnInbound;
    filter.bApplyBeforeOutbound = bApplyBeforeOutbound;
    filter.bApplyOnOutbound = bApplyOnOutbound;
    filter.bApplyOnExplicit = bApplyOnExplicit;
    filter.bApplyOnAllFolders = bApplyOnAllFolders;
    filter.bStopProcessingHere = bStopProcessingHere;
    filter.bConfigureShortcut = bConfigureShortcut;
    filter.bConfigureToolbar = bConfigureToolbar;
    filter.bAutoNaming = bAutoNaming;
    filter.bEnabled = bEnabled;
    filter.mApplicability = static_cast<MailCommon::MailFilter::AccountType>(applicability);

    return stream;
}

bool MailFilter::isEnabled() const
{
    return bEnabled;
}

void MailFilter::setEnabled(bool enabled)
{
    bEnabled = enabled;
}

/*
  Filter Dialog

  SPDX-FileCopyrightText: Marc Mutz <mutz@kde.org>
  SPDX-FileCopyrightText: 2011-2025 Laurent Montel <montel@kde.org>

  based upon work by Stefan Taferner <taferner@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kmfilterdialog.h"
#include "kmfilterlistbox.h"
#include "mailcommon_debug.h"

#include "filteractions/filteractionwidget.h"
#include "filterimporterpathcache.h"
#include "filterselectiondialog.h"
#include "kmfilteraccountlist.h"
using MailCommon::FilterImporterExporter;
#include "filterconverter/filterconverttosieve.h"
#include "filtermanager.h"
#include "folder/folderrequester.h"
#include "kernel/mailkernel.h"
#include "search/searchpatternedit.h"
#include <PimCommon/PimUtil>

#include <Akonadi/ItemFetchJob>

#include <KConfigGroup>

#include <KIconButton>
#include <KIconLoader>
#include <KJob>
#include <KKeySequenceWidget>
#include <KListWidgetSearchLine>
#include <KLocalizedString>
#include <KMessageBox>
#include <QIcon>
#include <QPushButton>
#include <QTabWidget>

#include <QButtonGroup>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QRadioButton>
#include <QShortcut>
#include <QSplitter>
#include <QTreeWidget>
#include <QVBoxLayout>

Q_DECLARE_METATYPE(MailCommon::FilterImporterExporter::FilterType)
using namespace MailCommon;

//=============================================================================
//
// class KMFilterDialog (the filter dialog)
//
//=============================================================================

KMFilterDialog::KMFilterDialog(const QList<KActionCollection *> &actionCollection, QWidget *parent, bool createDummyFilter)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Filter Rules"));
    auto mainLayout = new QVBoxLayout(this);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help | QDialogButtonBox::Apply, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    auto user1Button = new QPushButton(this);
    buttonBox->addButton(user1Button, QDialogButtonBox::ActionRole);
    auto user2Button = new QPushButton(this);
    buttonBox->addButton(user2Button, QDialogButtonBox::ActionRole);
    auto user3Button = new QPushButton(this);
    buttonBox->addButton(user3Button, QDialogButtonBox::ActionRole);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &KMFilterDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttonBox->button(QDialogButtonBox::Help), &QAbstractButton::clicked, this, &KMFilterDialog::slotHelp);
    setModal(false);
    okButton->setFocus();
    user1Button->setText(i18n("Import…"));
    user1Button->setIcon(QIcon::fromTheme("document-import"));
    user2Button->setText(i18n("Export…"));
    user2Button->setIcon(QIcon::fromTheme("document-export"));
    user3Button->setText(i18n("Convert to…"));
    user3Button->setIcon(QIcon::fromTheme("document-save-as"));
    auto menu = new QMenu(this);

    auto act = new QAction(i18nc("@action", "KMail filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::KMailFilter));
    menu->addAction(act);

    act = new QAction(i18nc("@action", "Thunderbird filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::ThunderBirdFilter));
    menu->addAction(act);

    act = new QAction(i18nc("@action", "Evolution filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::EvolutionFilter));
    menu->addAction(act);

    act = new QAction(i18nc("@action", "Sylpheed filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::SylpheedFilter));
    menu->addAction(act);

    act = new QAction(i18nc("@action", "Procmail filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::ProcmailFilter));
    menu->addAction(act);

    act = new QAction(i18nc("@action", "Balsa filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::BalsaFilter));
    menu->addAction(act);

    act = new QAction(i18nc("@action", "Claws Mail filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::ClawsMailFilter));
    menu->addAction(act);

    act = new QAction(i18nc("@action", "Icedove Mail filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::IcedoveFilter));
    menu->addAction(act);

    connect(menu, &QMenu::triggered, this, &KMFilterDialog::slotImportFilter);

    act = new QAction(i18nc("@action", "Gmail filters"), this);
    act->setData(QVariant::fromValue(MailCommon::FilterImporterExporter::GmailFilter));
    menu->addAction(act);

    user1Button->setMenu(menu);

    menu = new QMenu(this);

    act = new QAction(i18nc("@action", "Sieve script"), this);
    connect(act, &QAction::triggered, this, &KMFilterDialog::slotExportAsSieveScript);
    menu->addAction(act);
    user3Button->setMenu(menu);

    connect(user2Button, &QAbstractButton::clicked, this, &KMFilterDialog::slotExportFilters);
    mApplyButton = buttonBox->button(QDialogButtonBox::Apply);
    mApplyButton->setEnabled(false);

    auto w = new QWidget(this);
    mainLayout->addWidget(w);
    mainLayout->addWidget(buttonBox);
    auto topVLayout = new QVBoxLayout(w);
    topVLayout->setContentsMargins({});
    auto topLayout = new QHBoxLayout;
    topVLayout->addLayout(topLayout);
    topLayout->setContentsMargins({});

    auto splitter = new QSplitter;
    splitter->setChildrenCollapsible(false);
    topLayout->addWidget(splitter);

    mFilterList = new KMFilterListBox(i18n("Available Filters"), this);
    splitter->addWidget(mFilterList);
    auto tabWidget = new QTabWidget(this);
    splitter->addWidget(tabWidget);

    auto page1 = new QWidget(tabWidget);
    tabWidget->addTab(page1, i18nc("General mail filter settings.", "General"));
    auto hbl = new QHBoxLayout(page1);

    auto page2 = new QWidget(tabWidget);
    tabWidget->addTab(page2, i18nc("Advanced mail filter settings.", "Advanced"));
    auto vbl2 = new QVBoxLayout(page2);

    auto vbl = new QVBoxLayout();
    hbl->addLayout(vbl);
    hbl->setStretchFactor(vbl, 2);

    mPatternEdit = new MailCommon::SearchPatternEdit(page1, MailCommon::SearchPatternEdit::MatchAllMessages);
    vbl->addWidget(mPatternEdit, 0, Qt::AlignTop);

    auto actionLabel = new QLabel(i18nc("@label:textbox", "Filter actions:"), page1);
    vbl->addWidget(actionLabel);

    mActionLister = new MailCommon::FilterActionWidgetLister(page1);
    vbl->addWidget(mActionLister);

    mAdvOptsGroup = new QGroupBox(QString(), page2);
    mAdvOptsGroup->setFlat(true);
    mAdvOptsGroup->setContentsMargins({});
    {
        auto gl = new QGridLayout();
        mApplyOnIn = new QCheckBox(i18nc("@option:check", "Apply this filter to incoming messages:"), mAdvOptsGroup);
        gl->addWidget(mApplyOnIn, 0, 0);

        auto radioGroupWidget = new QWidget(page2);
        radioGroupWidget->setContentsMargins(20, 0, 0, 0);
        gl->addWidget(radioGroupWidget, 1, 0);
        auto vbl3 = new QVBoxLayout(radioGroupWidget);

        auto bg = new QButtonGroup(mAdvOptsGroup);

        mApplyOnForAll = new QRadioButton(i18nc("@option:radio", "From all accounts"), radioGroupWidget);
        bg->addButton(mApplyOnForAll);
        vbl3->addWidget(mApplyOnForAll);

        mApplyOnForTraditional = new QRadioButton(i18nc("@option:radio", "From all but online IMAP accounts"), radioGroupWidget);
        bg->addButton(mApplyOnForTraditional);
        vbl3->addWidget(mApplyOnForTraditional);

        mApplyOnForChecked = new QRadioButton(i18nc("@option:radio", "From selected accounts only"), radioGroupWidget);
        bg->addButton(mApplyOnForChecked);
        vbl3->addWidget(mApplyOnForChecked);
        vbl3->addStretch(100);

        mAccountList = new KMFilterAccountList(mAdvOptsGroup);
        gl->addWidget(mAccountList, 0, 1, 4, 3);

        mApplyOnOut = new QCheckBox(i18nc("@option:check", "Apply this filter to &sent messages"), mAdvOptsGroup);
        mApplyOnOut->setToolTip(
            i18n("<p>The filter will be triggered <b>after</b> the message is sent "
                 "and it will only affect the local copy of the message.</p>"
                 "<p>If the recipient's copy also needs to be modified, "
                 "please use \"Apply this filter <b>before</b> sending messages\".</p>"));
        gl->addWidget(mApplyOnOut, 5, 0, 1, 4);

        mApplyBeforeOut = new QCheckBox(i18nc("@option:check", "Apply this filter &before sending messages"), mAdvOptsGroup);
        mApplyBeforeOut->setToolTip(
            i18n("<p>The filter will be triggered <b>before</b> the message is sent "
                 "and it will affect both the local copy and the sent copy of the message.</p>"
                 "<p>This is required if the recipient's copy also needs to be modified.</p>"));
        gl->addWidget(mApplyBeforeOut, 6, 0, 1, 4);

        mApplyOnCtrlJ = new QCheckBox(i18nc("@option:check", "Apply this filter on manual &filtering"), mAdvOptsGroup);
        gl->addWidget(mApplyOnCtrlJ, 7, 0, 1, 4);

        mApplyOnAllFolders = new QCheckBox(i18nc("@option:check", "Apply this filter on inbound emails in all folders"), mAdvOptsGroup);
        mApplyOnAllFolders->setToolTip(
            i18n("<p>The filter will be applied on inbound emails from all folders "
                 "belonging to all accounts selected above. This is useful when using local filters "
                 "with IMAP accounts where new emails may have already been moved to different folders "
                 "by server-side filters.</p>"));
        gl->addWidget(mApplyOnAllFolders, 8, 0, 1, 4);

        mStopProcessingHere = new QCheckBox(i18nc("@option:check", "If this filter &matches, stop processing here"), mAdvOptsGroup);
        gl->addWidget(mStopProcessingHere, 9, 0, 1, 4);

        mConfigureShortcut = new QCheckBox(i18nc("@option:check", "Add this filter to the Apply Filter menu"), mAdvOptsGroup);
        gl->addWidget(mConfigureShortcut, 10, 0, 1, 2);

        mInMenuWidget = new QWidget(mAdvOptsGroup);
        mInMenuWidget->setContentsMargins(20, 0, 0, 0);
        gl->addWidget(mInMenuWidget, 11, 0, 1, 4);
        auto inMenuLayout = new QFormLayout(mInMenuWidget);

        connect(mConfigureShortcut, &QCheckBox::toggled, this, [this](bool aChecked) {
            if (mFilter) {
                mFilter->setConfigureShortcut(aChecked);
                mInMenuWidget->setEnabled(aChecked);

                // Enable the apply button
                slotDialogUpdated();
            }
        });

        mConfigureToolbar = new QCheckBox(i18nc("@option:check", "Additionally add this filter to the toolbar"), mInMenuWidget);
        inMenuLayout->addRow(mConfigureToolbar);

        mKeySeqWidget = new KKeySequenceWidget(mInMenuWidget);
        mKeySeqWidget->setObjectName(QLatin1StringView("FilterShortcutSelector"));
#if KXMLGUI_VERSION >= QT_VERSION_CHECK(6, 12, 0)
        mKeySeqWidget->setPatterns(KKeySequenceRecorder::Key);
#else
        mKeySeqWidget->setModifierlessAllowed(true);
#endif
        mKeySeqWidget->setCheckActionCollections(actionCollection);
        inMenuLayout->addRow(i18n("Shortcut:"), mKeySeqWidget);

        mFilterActionIconButton = new KIconButton(mInMenuWidget);
        mFilterActionIconButton->setIconType(KIconLoader::NoGroup, KIconLoader::Action, false);
        mFilterActionIconButton->setIconSize(16);
        mFilterActionIconButton->setIcon(QIcon::fromTheme(QStringLiteral("system-run")));
        inMenuLayout->addRow(i18n("Icon for this filter:"), mFilterActionIconButton);

        mAdvOptsGroup->setLayout(gl);
        mInMenuWidget->setEnabled(false);
    }
    vbl2->addWidget(mAdvOptsGroup, 0, Qt::AlignTop);

    auto applySpecificFiltersLayout = new QHBoxLayout;
    auto lab = new QLabel(i18nc("@label:textbox", "Run selected filter(s) on: "), this);
    applySpecificFiltersLayout->addWidget(lab);
    mFolderRequester = new MailCommon::FolderRequester(this);
    mFolderRequester->setNotAllowToCreateNewFolder(true);
    applySpecificFiltersLayout->addWidget(mFolderRequester);
    connect(mFolderRequester, &FolderRequester::folderChanged, this, &KMFilterDialog::slotFolderChanged);
    mRunNow = new QPushButton(i18nc("@action:button", "Run Now"), this);
    mRunNow->setEnabled(false);
    applySpecificFiltersLayout->addWidget(mRunNow);
    connect(mRunNow, &QPushButton::clicked, this, &KMFilterDialog::slotRunFilters);
    topVLayout->addLayout(applySpecificFiltersLayout);
    // spacer:
    vbl->addStretch(1);

    // load the filter parts into the edit widgets
    connect(mFilterList, &KMFilterListBox::filterSelected, this, &KMFilterDialog::slotFilterSelected);

    // transfer changes from the 'Apply this filter on...'
    // combo box to the filter
    connect(mApplyOnIn, &QCheckBox::clicked, this, &KMFilterDialog::slotApplicabilityChanged);
    connect(mApplyOnForAll, &QRadioButton::clicked, this, &KMFilterDialog::slotApplicabilityChanged);
    connect(mApplyOnForTraditional, &QAbstractButton::clicked, this, &KMFilterDialog::slotApplicabilityChanged);
    connect(mApplyOnForChecked, &QAbstractButton::clicked, this, &KMFilterDialog::slotApplicabilityChanged);
    connect(mApplyBeforeOut, &QAbstractButton::clicked, this, &KMFilterDialog::slotApplicabilityChanged);
    connect(mApplyOnAllFolders, &QAbstractButton::clicked, this, &KMFilterDialog::slotApplicabilityChanged);
    connect(mApplyOnOut, &QAbstractButton::clicked, this, &KMFilterDialog::slotApplicabilityChanged);
    connect(mApplyOnCtrlJ, &QAbstractButton::clicked, this, &KMFilterDialog::slotApplicabilityChanged);
    connect(mAccountList, &KMFilterAccountList::itemChanged, this, &KMFilterDialog::slotApplicableAccountsChanged);

    // transfer changes from the 'stop processing here'
    // check box to the filter
    connect(mStopProcessingHere, &QCheckBox::toggled, this, &KMFilterDialog::slotStopProcessingButtonToggled);

    connect(mKeySeqWidget, &KKeySequenceWidget::keySequenceChanged, this, &KMFilterDialog::slotShortcutChanged);

    connect(mConfigureToolbar, &QCheckBox::toggled, this, &KMFilterDialog::slotConfigureToolbarButtonToggled);

    connect(mFilterActionIconButton, &KIconButton::iconChanged, this, &KMFilterDialog::slotFilterActionIconChanged);

    // reset all widgets here
    connect(mFilterList, &KMFilterListBox::resetWidgets, this, &KMFilterDialog::slotReset);

    connect(mFilterList, &KMFilterListBox::applyWidgets, this, &KMFilterDialog::slotUpdateFilter);

    // support auto-naming the filter
    connect(mPatternEdit, &MailCommon::SearchPatternEdit::maybeNameChanged, mFilterList, &KMFilterListBox::slotUpdateFilterName);

    // save filters on 'Apply' or 'OK'
    connect(mApplyButton, &QAbstractButton::clicked, this, &KMFilterDialog::slotApply);

    // save dialog size on 'OK'
    connect(okButton, &QAbstractButton::clicked, this, &KMFilterDialog::slotSaveSize);

    // destruct the dialog on close and Cancel
    connect(buttonBox->button(QDialogButtonBox::Cancel), &QAbstractButton::clicked, this, &KMFilterDialog::slotFinished);

    // disable closing when user wants to continue editing
    connect(mFilterList, &KMFilterListBox::abortClosing, this, &KMFilterDialog::slotDisableAccept);

    connect(mFilterList, &KMFilterListBox::filterCreated, this, &KMFilterDialog::slotDialogUpdated);
    connect(mFilterList, &KMFilterListBox::filterRemoved, this, &KMFilterDialog::slotDialogUpdated);
    connect(mFilterList, &KMFilterListBox::filterUpdated, this, &KMFilterDialog::slotDialogUpdated);
    connect(mFilterList, &KMFilterListBox::filterOrderAltered, this, &KMFilterDialog::slotDialogUpdated);
    connect(mPatternEdit, &MailCommon::SearchPatternEdit::patternChanged, this, &KMFilterDialog::slotDialogUpdated);
    connect(mActionLister, qOverload<QWidget *>(&FilterActionWidgetLister::widgetAdded), this, &KMFilterDialog::slotDialogUpdated);
    connect(mActionLister, qOverload<QWidget *>(&FilterActionWidgetLister::widgetRemoved), this, &KMFilterDialog::slotDialogUpdated);
    connect(mActionLister, &MailCommon::FilterActionWidgetLister::filterModified, this, &KMFilterDialog::slotDialogUpdated);
    connect(mActionLister, &MailCommon::FilterActionWidgetLister::clearWidgets, this, &KMFilterDialog::slotDialogUpdated);
    KConfigGroup myGroup(KSharedConfig::openStateConfig(), QStringLiteral("Geometry"));
    const QSize size = myGroup.readEntry("filterDialogSize", QSize());
    if (size != QSize()) {
        resize(size);
    } else {
        adjustSize();
    }

    // load the filter list (emits filterSelected())
    mFilterList->loadFilterList(createDummyFilter);
    mIgnoreFilterUpdates = false;
}

void KMFilterDialog::createFilter(const QByteArray &field, const QString &value)
{
    mFilterList->createFilter(field, value);
}

void KMFilterDialog::accept()
{
    if (mDoNotClose) {
        mDoNotClose = false; // only abort current close attempt
    } else {
        QDialog::accept();
        slotFinished();
    }
}

bool KMFilterDialog::event(QEvent *e)
{
    // Close the bar when pressing Escape.
    // Not using a QShortcut for this because it could conflict with
    // window-global actions (e.g. Emil Sedgh binds Esc to "close tab").
    // With a shortcut override we can catch this before it gets to kactions.
    const bool shortCutOverride = (e->type() == QEvent::ShortcutOverride);
    if (shortCutOverride || e->type() == QEvent::KeyPress) {
        auto kev = static_cast<QKeyEvent *>(e);
        if (kev->key() == Qt::Key_Escape) {
            e->ignore();
            return true;
        }
    }
    return QDialog::event(e);
}

void KMFilterDialog::slotApply()
{
    mApplyButton->setEnabled(false);
    mFilterList->slotApplied();
}

void KMFilterDialog::slotFinished()
{
    deleteLater();
}

void KMFilterDialog::slotFolderChanged(const Akonadi::Collection &collection)
{
    mRunNow->setEnabled(collection.isValid());
}

void KMFilterDialog::slotRunFilters()
{
    if (!mFolderRequester->collection().isValid()) {
        KMessageBox::information(this,
                                 i18nc("@info", "Unable to apply this filter since there are no folders selected."),
                                 i18nc("@title:window", "No folder selected."));
        return;
    }

    if (mApplyButton->isEnabled()) {
        KMessageBox::information(this,
                                 i18nc("@info",
                                       "Some filters were changed and not saved yet. "
                                       "You must save your filters before they can be applied."),
                                 i18nc("@title:window", "Filters changed."));
        return;
    }
    SearchRule::RequiredPart requiredPart = SearchRule::Envelope;
    const QStringList selectedFiltersId = mFilterList->selectedFilterId(requiredPart, mFolderRequester->collection().resource());
    if (selectedFiltersId.isEmpty()) {
        KMessageBox::information(this,
                                 i18nc("@info", "Unable to apply a filter since there are no filters currently selected."),
                                 i18nc("@title:window", "No filters selected."));
        return;
    }
    auto job = new Akonadi::ItemFetchJob(mFolderRequester->collection(), this);
    job->setProperty("requiredPart", QVariant::fromValue(requiredPart));
    job->setProperty("listFilters", QVariant::fromValue(selectedFiltersId));

    connect(job, &KJob::result, this, &KMFilterDialog::slotFetchItemsForFolderDone);

    mRunNow->setEnabled(false); // Disable it
}

void KMFilterDialog::slotFetchItemsForFolderDone(KJob *job)
{
    auto fjob = qobject_cast<Akonadi::ItemFetchJob *>(job);
    Q_ASSERT(fjob);

    QStringList filtersId;
    if (fjob->property("listFilters").isValid()) {
        filtersId = fjob->property("listFilters").toStringList();
    }

    SearchRule::RequiredPart requiredPart = SearchRule::Envelope;
    if (fjob->property("requiredPart").isValid()) {
        requiredPart = fjob->property("requiredPart").value<SearchRule::RequiredPart>();
    }
    Akonadi::Item::List items = fjob->items();
    mRunNow->setEnabled(true);
    MailCommon::FilterManager::instance()->filter(items, requiredPart, filtersId);
}

void KMFilterDialog::slotSaveSize()
{
    mFilterList->slotAccepted();
    KConfigGroup myGroup(KSharedConfig::openStateConfig(), QStringLiteral("Geometry"));
    myGroup.writeEntry("filterDialogSize", size());
    myGroup.sync();
}

void KMFilterDialog::slotFilterSelected(MailFilter *aFilter)
{
    Q_ASSERT(aFilter);
    mIgnoreFilterUpdates = true;
    mActionLister->setActionList(aFilter->actions());

    mAdvOptsGroup->setEnabled(true);

    mPatternEdit->setSearchPattern(aFilter->pattern());
    mFilter = aFilter;

    qCDebug(MAILCOMMON_LOG) << "apply on inbound ==" << aFilter->applyOnInbound();
    qCDebug(MAILCOMMON_LOG) << "apply on outbound ==" << aFilter->applyOnOutbound();
    qCDebug(MAILCOMMON_LOG) << "apply before outbound == " << aFilter->applyBeforeOutbound();
    qCDebug(MAILCOMMON_LOG) << "apply on explicit ==" << aFilter->applyOnExplicit();
    qCDebug(MAILCOMMON_LOG) << "apply on all folders inboud == " << aFilter->applyOnAllFoldersInbound();

    // NOTE: setting these values activates the slot that sets them in
    // the filter! So make sure we have the correct values _before_ we
    // set the first one:
    const bool applyOnIn = aFilter->applyOnInbound();
    const bool applyOnForAll = aFilter->applicability() == MailFilter::All;
    const bool applyOnTraditional = aFilter->applicability() == MailFilter::ButImap;
    const bool applyBeforeOut = aFilter->applyBeforeOutbound();
    const bool applyOnOut = aFilter->applyOnOutbound();
    const bool applyOnAllFolders = aFilter->applyOnAllFoldersInbound();
    const bool applyOnExplicit = aFilter->applyOnExplicit();
    const bool stopHere = aFilter->stopProcessingHere();
    const bool configureShortcut = aFilter->configureShortcut();
    const bool configureToolbar = aFilter->configureToolbar();
    const QString icon = aFilter->icon();
    const QKeySequence shortcut(aFilter->shortcut());

    mApplyOnIn->setChecked(applyOnIn);
    mApplyOnForAll->setEnabled(applyOnIn);
    mApplyOnForTraditional->setEnabled(applyOnIn);
    mApplyOnForChecked->setEnabled(applyOnIn);
    mApplyOnForAll->setChecked(applyOnForAll);
    mApplyOnAllFolders->setChecked(applyOnAllFolders);
    mApplyOnForTraditional->setChecked(applyOnTraditional);
    mApplyOnForChecked->setChecked(!applyOnForAll && !applyOnTraditional);
    mAccountList->setEnabled(mApplyOnForChecked->isEnabled() && mApplyOnForChecked->isChecked());
    slotUpdateAccountList();
    mApplyBeforeOut->setChecked(applyBeforeOut);
    mApplyOnOut->setChecked(applyOnOut);
    mApplyOnCtrlJ->setChecked(applyOnExplicit);
    mStopProcessingHere->setChecked(stopHere);
    mConfigureShortcut->setChecked(configureShortcut);
    mKeySeqWidget->setKeySequence(shortcut, KKeySequenceWidget::NoValidate);
    mConfigureToolbar->setChecked(configureToolbar);
    mFilterActionIconButton->setIcon(icon);
    mIgnoreFilterUpdates = false;
}

void KMFilterDialog::slotReset()
{
    mFilter = nullptr;
    mPatternEdit->reset();

    mActionLister->reset();
    mAdvOptsGroup->setEnabled(false);
    slotUpdateAccountList();
}

void KMFilterDialog::slotUpdateFilter()
{
    mPatternEdit->updateSearchPattern();
    mActionLister->updateActionList();
}

void KMFilterDialog::slotApplicabilityChanged()
{
    if (mFilter) {
        mFilter->setApplyOnInbound(mApplyOnIn->isChecked());
        mFilter->setApplyBeforeOutbound(mApplyBeforeOut->isChecked());
        mFilter->setApplyOnOutbound(mApplyOnOut->isChecked());
        mFilter->setApplyOnExplicit(mApplyOnCtrlJ->isChecked());
        mFilter->setApplyOnAllFoldersInbound(mApplyOnAllFolders->isChecked());
        if (mApplyOnForAll->isChecked()) {
            mFilter->setApplicability(MailFilter::All);
            mFilter->clearApplyOnAccount();
        } else if (mApplyOnForTraditional->isChecked()) {
            mFilter->setApplicability(MailFilter::ButImap);
        } else if (mApplyOnForChecked->isChecked()) {
            mFilter->setApplicability(MailFilter::Checked);
        }

        mApplyOnForAll->setEnabled(mApplyOnIn->isChecked());
        mApplyOnForTraditional->setEnabled(mApplyOnIn->isChecked());
        mApplyOnForChecked->setEnabled(mApplyOnIn->isChecked());
        mAccountList->setEnabled(mApplyOnForChecked->isEnabled() && mApplyOnForChecked->isChecked());

        // Advanced tab functionality - Update list of accounts this filter applies to
        if (!mApplyOnForAll->isChecked()) {
            mAccountList->reloadAndSelectAccountsFrom(mFilter);
        }

        // Enable the apply button
        slotDialogUpdated();

        qCDebug(MAILCOMMON_LOG) << "Setting filter to be applied at" << (mFilter->applyOnInbound() ? "incoming " : "")
                                << (mFilter->applyOnOutbound() ? "outgoing " : "") << (mFilter->applyBeforeOutbound() ? "before_outgoing " : "")
                                << (mFilter->applyOnAllFoldersInbound() ? "all folders inboud " : "") << (mFilter->applyOnExplicit() ? "explicit CTRL-J" : "");
    }
}

void KMFilterDialog::slotApplicableAccountsChanged()
{
    // Advanced tab functionality - Update list of accounts this filter applies to
    if (mFilter && mApplyOnForChecked->isEnabled() && mApplyOnForChecked->isChecked()) {
        QTreeWidgetItemIterator it(mAccountList);

        while (QTreeWidgetItem *item = *it) {
            const QString id = item->text(2);
            mFilter->setApplyOnAccount(id, item->checkState(0) == Qt::Checked);
            ++it;
        }

        // Enable the apply button
        slotDialogUpdated();
    }
}

void KMFilterDialog::slotStopProcessingButtonToggled(bool aChecked)
{
    if (mFilter) {
        mFilter->setStopProcessingHere(aChecked);

        // Enable the apply button
        slotDialogUpdated();
    }
}

void KMFilterDialog::slotShortcutChanged(const QKeySequence &newSeq)
{
    if (mFilter) {
        mKeySeqWidget->applyStealShortcut();
        mFilter->setShortcut(newSeq);

        // Enable the apply button
        slotDialogUpdated();
    }
}

void KMFilterDialog::slotConfigureToolbarButtonToggled(bool aChecked)
{
    if (mFilter) {
        mFilter->setConfigureToolbar(aChecked);
        // Enable the apply button
        slotDialogUpdated();
    }
}

void KMFilterDialog::slotFilterActionIconChanged(const QString &icon)
{
    if (mFilter) {
        mFilter->setIcon(icon);
        // Enable the apply button
        slotDialogUpdated();
    }
}

void KMFilterDialog::slotUpdateAccountList()
{
    mAccountList->reloadAndSelectAccountsFrom(mFilter);
}

void KMFilterDialog::slotImportFilter(QAction *act)
{
    if (act) {
        importFilters(act->data().value<MailCommon::FilterImporterExporter::FilterType>());
    }
}

void KMFilterDialog::importFilters(MailCommon::FilterImporterExporter::FilterType type)
{
    MailCommon::FilterImporterPathCache::self()->clear();
    FilterImporterExporter importer(this);
    bool canceled = false;
    QList<MailFilter *> filters = importer.importFilters(canceled, type);
    if (canceled) {
        return;
    }

    if (filters.isEmpty()) {
        KMessageBox::information(this, i18n("No filter was imported."));
        return;
    }
    QStringList listOfFilter;
    QList<MailFilter *>::ConstIterator end(filters.constEnd());

    for (QList<MailFilter *>::ConstIterator it = filters.constBegin(); it != end; ++it) {
        mFilterList->appendFilter(*it); // no need to deep copy, ownership passes to the list
        listOfFilter << (*it)->name();
    }

    KMessageBox::informationList(this, i18n("Filters which were imported:"), listOfFilter);
}

void KMFilterDialog::slotExportFilters()
{
    bool wasCanceled = false;
    const QList<MailFilter *> filters = mFilterList->filtersForSaving(false, wasCanceled);
    if (filters.isEmpty()) {
        KMessageBox::information(this, i18n("Any filter found."));
        return;
    }
    if (wasCanceled) {
        qDeleteAll(filters);
        return;
    }
    FilterImporterExporter exporter(this);
    exporter.exportFilters(filters);
}

void KMFilterDialog::slotDisableAccept()
{
    mDoNotClose = true;
}

void KMFilterDialog::slotDialogUpdated()
{
    qCDebug(MAILCOMMON_LOG) << "Detected a change in data bound to the dialog!";
    if (!mIgnoreFilterUpdates) {
        mApplyButton->setEnabled(true);
    }
}

void KMFilterDialog::slotExportAsSieveScript()
{
    if (mApplyButton->isEnabled()) {
        KMessageBox::information(this,
                                 i18nc("@info",
                                       "Some filters were changed and not saved yet.<br>"
                                       "You must save your filters before they can be exported."),
                                 i18nc("@title:window", "Filters changed."));
        return;
    }
    KMessageBox::information(this,
                             i18n("We cannot convert all KMail filters to sieve scripts but we can try :)"),
                             i18nc("@title:window", "Convert KMail filters to sieve scripts"));
    bool wasCanceled = false;
    const QList<MailFilter *> filters = mFilterList->filtersForSaving(false, wasCanceled);
    if (filters.isEmpty()) {
        return;
    }
    if (!wasCanceled) {
        QPointer<FilterSelectionDialog> dlg = new FilterSelectionDialog(this);
        dlg->setFilters(filters);
        if (dlg->exec() == QDialog::Accepted) {
            QList<MailFilter *> lst = dlg->selectedFilters();
            if (!lst.isEmpty()) {
                FilterConvertToSieve convert(lst);
                convert.convert();
                qDeleteAll(lst);
            } else {
                KMessageBox::information(this, i18n("No filters selected."), i18nc("@title:window", "Convert KMail filters to sieve scripts"));
            }
        }
        delete dlg;
    } else {
        qDeleteAll(filters);
    }
}

void KMFilterDialog::slotHelp()
{
    PimCommon::Util::invokeHelp(QStringLiteral("kmail2/filters.html"));
}

#include "moc_kmfilterdialog.cpp"

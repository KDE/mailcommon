/*
  Filter Dialog

  SPDX-FileCopyrightText: Marc Mutz <mutz@kde.org>
  SPDX-FileCopyrightText: 2011-2026 Laurent Montel <montel@kde.org>

  based upon work by Stefan Taferner <taferner@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "filterimporterexporter.h"
#include "mailcommon/filteraction.h"
#include "mailcommon/searchpattern.h"
#include "mailcommon_export.h"
#include "mailfilter.h"

#include <QDialog>

#include <QList>

class KActionCollection;
class KIconButton;
class KKeySequenceWidget;

class QCheckBox;
class QPushButton;
class QRadioButton;
class QGroupBox;
namespace MailCommon
{
class SearchPatternEdit;
class FilterActionWidgetLister;
class FolderRequester;
class KMFilterAccountList;
class KMFilterListBox;
}

class KJob;

/*!
 * \class MailCommon::KMFilterDialog
 * \inmodule MailCommon
 * \inheaderfile MailCommon/KMFilterDialog
 *
 * The filter dialog. This is a non-modal dialog used to manage the filters.
 * It should only be called through the filter manager. The dialog
 * consists of three main parts:
 *
 * \list
 * \li The \c KMFilterListBox in the left half allows the user to
 * select a filter to be displayed using the widgets on the right
 * half. It also has buttons to delete filters, add new ones, to
 * rename them and to change their order (maybe you will be able to
 * move the filters around by dragging later, and to optimize the
 * filters by trying to apply them to all locally available
 * KMMessage in turn and thus profiling which filters (and which
 * rules of the search patterns) matches most often and sorting the
 * filter/rules list according to the results, but I first want the
 * basic functionality in place).
 * \li The SearchPatternEdit in the upper-right quarter allows
 * the user to modify the filter criteria.
 * \li The \c FilterActionWidgetLister in the lower-right quarter allows
 * the user to select the actions that will be executed for any
 * message that matches the search pattern.
 * \li (tbi) There will be another widget that will allow the user to
 * select to which folders the filter may be applied and whether it
 * should be applied on outbound or inbound message transfers or both
 * or none (meaning the filter is only applied when the user
 * explicitly hits CTRL-J). I'm not sure whether there should be a
 * per-folder filter list or a single list where you can select the
 * names of folders this rule will be applied to.
 * \endlist
 *
 * Upon creating the dialog, a (deep) copy of the current filter list
 * is made by KMFilterListBox. The changed filters are local to
 * KMFilterListBox until the user clicks the 'Apply' button.
 *
 * \note Though this dialog is non-modal, it completely ignores all
 * the stuff that goes on behind the scenes with folders esp. folder
 * creation, move and create. The widgets that depend on the filter
 * list and the filters that use folders as parameters are not
 * updated as you expect. I hope this will change sometime soon.
 *
 * KMFilterDialog supports the creation of new filters through context
 * menus, dubbed "rapid filters". Call \c {IFilter::createFilter()}
 * to use this. That call will be delivered to this dialog, which in
 * turn delivers it to the KMFilterListBox.
 *
 * If you change the (DocBook) anchor for the filter dialog help,
 * make sure to change \c KMFilterDialogHelpAnchor
 * in kmfilterdlg.cpp accordingly.
 *
 * \brief The filter dialog.
 *
 * \author Marc Mutz <mutz@kde.org>, based upon work by Stefan Taferner <taferner@kde.org>.
 * \sa MailCommon::MailFilter MailCommon::SearchPatternEdit
 */
namespace MailCommon
{
class MAILCOMMON_EXPORT KMFilterDialog : public QDialog
{
    Q_OBJECT

public:
    /*!
     * Creates the filter dialog with the given \a parent. The actions of
     * \a actionCollection can be used as shortcuts for filters. If
     * \a createDummyFilter is \c true, an empty filter is created when there is
     * no filter. Only the filter manager should create this dialog. This
     * ensures that there is only a single filter dialog.
     */
    explicit KMFilterDialog(const QList<KActionCollection *> &actionCollection, QWidget *parent = nullptr, bool createDummyFilter = true);

    /*!
     * Called from the filter manager. Creates a new filter and presets
     * the first rule with "field equals value", using \a field and \a value.
     */
    void createFilter(const QByteArray &field, const QString &value);

public Q_SLOTS:
    /*!
     * Internally connected to the filter list's filterSelected() signal.
     * Loads the pattern and the actions of \a aFilter into the edit widgets.
     */
    void slotFilterSelected(MailCommon::MailFilter *aFilter);

    /*! Override QDialog::accept to allow disabling close */
    void accept() override;

protected Q_SLOTS:
    /*!
     * Called when the applicability of the current filter changes.
     */
    void slotApplicabilityChanged();
    /*!
     * Called when the accounts the current filter applies to change.
     */
    void slotApplicableAccountsChanged();
    /*!
     * Called when the "stop processing here" check box is toggled to \a aChecked.
     */
    void slotStopProcessingButtonToggled(bool aChecked);
    /*!
     * Called when the shortcut of the current filter changes to \a newSeq.
     */
    void slotShortcutChanged(const QKeySequence &newSeq);
    /*!
     * Called when the "add to toolbar" check box is toggled to \a aChecked.
     */
    void slotConfigureToolbarButtonToggled(bool aChecked);
    /*!
     * Called when the toolbar \a icon of the current filter changes.
     */
    void slotFilterActionIconChanged(const QString &icon);
    /*!
     * Resets the edit widgets.
     */
    void slotReset();
    /*!
     * Applies the content of the edit widgets to the current filter.
     */
    void slotUpdateFilter();
    /*!
     * Saves the dialog size.
     */
    void slotSaveSize();

    /*!
     * Called when the dialog is closed (finished).
     */
    void slotFinished();

    /*!
     * Update the list of accounts shown in the advanced tab.
     */
    void slotUpdateAccountList();

    /*!
     * Called when a user clicks the import filters button. Pops up
     * a dialog asking the user which file to import from and which
     * of the filters in that file to import.
     */
    void slotImportFilter(QAction *);

    /*!
     * Called when a user clicks the export filters button. Pops up
     * a dialog asking the user which filters to export and which
     * file to export to.
     */
    void slotExportFilters();

    /*!
     * Called when a user decides to continue editing invalid filters
     */
    void slotDisableAccept();

    /*!
     * Called whenever a change in the filters configuration is detected,
     * to enable the Apply button.
     */
    void slotDialogUpdated();

    /*!
     * Called wherenever the apply button is pressed.
     */
    void slotApply();

    /*!
     * Runs the selected filters on the selected folder.
     */
    void slotRunFilters();

    /*!
     * Called when the item fetch \a job for the selected folder is done.
     */
    void slotFetchItemsForFolderDone(KJob *job);

    /*!
     * Called when the selected folder changes.
     */
    void slotFolderChanged(const Akonadi::Collection &);

private:
    MAILCOMMON_NO_EXPORT void slotExportAsSieveScript();
    MAILCOMMON_NO_EXPORT void slotHelp();
    MAILCOMMON_NO_EXPORT void importFilters(MailCommon::FilterImporterExporter::FilterType type);

protected:
    bool event(QEvent *e) override;

    /*! The widget that contains the ListBox showing the filters, and the
        controls to remove filters, add new ones and to change their order. */
    KMFilterListBox *mFilterList = nullptr;

    /*! The widget that allows editing of the filter pattern. */
    MailCommon::SearchPatternEdit *mPatternEdit = nullptr;

    /*! The widget that allows editing of the filter actions. */
    MailCommon::FilterActionWidgetLister *mActionLister = nullptr;

    /*! Lets the user select whether to apply this filter on
       inbound/outbound messages, both, or only on explicit CTRL-J. */
    QCheckBox *mApplyOnIn = nullptr;
    QCheckBox *mApplyOnOut = nullptr;
    QCheckBox *mApplyBeforeOut = nullptr;
    QCheckBox *mApplyOnCtrlJ = nullptr;
    QCheckBox *mApplyOnAllFolders = nullptr;

    /*! For a filter applied to inbound messages selects whether to apply
        this filter to all accounts or to selected accounts only. */
    QRadioButton *mApplyOnForAll = nullptr;
    QRadioButton *mApplyOnForTraditional = nullptr;
    QRadioButton *mApplyOnForChecked = nullptr;

    /*! ListView that shows the accounts in the advanced tab */
    KMFilterAccountList *mAccountList = nullptr;

    QCheckBox *mStopProcessingHere = nullptr;
    QCheckBox *mConfigureShortcut = nullptr;
    QCheckBox *mConfigureToolbar = nullptr;
    KIconButton *mFilterActionIconButton = nullptr;
    KKeySequenceWidget *mKeySeqWidget = nullptr;
    QGroupBox *mAdvOptsGroup = nullptr;

    MailCommon::MailFilter *mFilter = nullptr;
    MailCommon::FolderRequester *mFolderRequester = nullptr;
    QPushButton *mRunNow = nullptr;
    QPushButton *mApplyButton = nullptr;
    bool mDoNotClose = false;
    bool mIgnoreFilterUpdates = true;
    QWidget *mInMenuWidget = nullptr;
};
}

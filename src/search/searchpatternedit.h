/*
  SPDX-FileCopyrightText: Marc Mutz <mutz@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_SEARCHPATTERNEDIT_H
#define MAILCOMMON_SEARCHPATTERNEDIT_H

#include "mailcommon_export.h"
#include "searchpattern.h"

#include <Libkdepim/KWidgetLister>

#include <QByteArray>
#include <QGroupBox>

class KComboBox;
class QPushButton;

class QAbstractButton;
class QRadioButton;
class QStackedWidget;

namespace MailCommon
{
class SearchPatternEdit;

/**
 * This widget is intended to be used in the filter configuration as
 * well as in the message search dialogs. It consists of a frame,
 * inside which there are placed two radio buttons entitled "Match
 * {all,any} of the following", followed by a vertical stack of
 * MailCommon::SearchRuleWidgets (initially two) and two buttons to add
 * and remove, resp., additional KMSearchWidget 's.
 *
 * To set the widget according to a given KMSearchPattern, use
 * setSearchPattern; to initialize it (e.g. for a new, virgin
 * rule), use setSearchPattern with a 0 argument. The widget
 * operates directly on a shallow(!) copy of the search rule. So
 * while you actually don't really need searchPattern, because
 * you can always store a pointer to the current pattern yourself,
 * you must not modify the currently-worked-on pattern yourself while
 * this widget holds a reference to it. The only exceptions are:
 *
 * @li If you edit a derived class, you can change aspects of the
 * class that don't interfere with the KMSearchPattern part. An
 * example is KMFilter, whose actions you can still edit while the
 * KMSearchPattern part of it is being acted upon by this widget.
 *
 * @li You can change the name of the pattern, but only using (this
 * widget's) setName. You cannot change the pattern's name
 * directly, although this widget in itself doesn't let the user
 * change it. This is because it auto-names the pattern to
 * "<$field>:$contents" iff the pattern begins with "<".
 *
 * @short A widget which allows editing a set of MailCommon::SearchRule's.
 * @author Marc Mutz <mutz@kde.org>
 */
class SearchRuleWidgetLister;
class MAILCOMMON_EXPORT SearchPatternEdit : public QWidget
{
    Q_OBJECT

public:
    enum SearchPatternEditOption {
        None = 0,
        HeadersOnly = 1,
        NotShowAbsoluteDate = 2,
        MatchAllMessages = 4,
        NotShowSize = 8,
        NotShowDate = 16,
        NotShowTags = 32
    };
    Q_DECLARE_FLAGS(SearchPatternEditOptions, SearchPatternEditOption)

    enum SearchModeType { StandardMode = 0, BalooMode = 1 };

    /**
     * Constructor. The parent parameter is passed to the underlying
     * QGroupBox, as usual.
     */
    explicit SearchPatternEdit(QWidget *parent = nullptr,
                               SearchPatternEditOptions options = (SearchPatternEditOptions)(None),
                               SearchModeType modeType = StandardMode);

    ~SearchPatternEdit() override;

    void setPatternEditOptions(SearchPatternEdit::SearchPatternEditOptions options);

    /**
     * Sets the search pattern. Rules are inserted regardless of the
     * return value of each rules' MailCommon::SearchRule::isEmpty.
     * This widget makes a shallow copy of @p aPattern and operates
     * directly on it.
     */
    void setSearchPattern(MailCommon::SearchPattern *aPattern);

    /**
     * Updates the search pattern according to the current widget values.
     */
    void updateSearchPattern();

public Q_SLOTS:
    /**
     * Called when the widget should let go of the currently referenced
     * filter and disable itself.
     */
    void reset();

Q_SIGNALS:
    /**
     * This signal is emitted whenever the name of the processed
     * search pattern may have changed.
     */
    void maybeNameChanged();

    /**
     * This signal is emitted wherenever the search pattern changes in some way.
     */
    void patternChanged();

    void returnPressed();

private Q_SLOTS:
    void slotRadioClicked(QAbstractButton *aRBtn);
    void slotAutoNameHack();
    void slotRuleAdded(QWidget *widget);

private:
    void initLayout(SearchPatternEditOptions options, SearchModeType modeType);
    MailCommon::SearchPattern *mPattern = nullptr;
    QRadioButton *mAllRBtn = nullptr;
    QRadioButton *mAnyRBtn = nullptr;
    QRadioButton *mAllMessageRBtn = nullptr;
    SearchRuleWidgetLister *mRuleLister = nullptr;
};

/**
 * A widget to edit a single MailCommon::SearchRule.
 * It consists of an editable KComboBox for the field,
 * a read-only KComboBox for the function and
 * a QLineEdit for the content or the pattern (in case of regexps).
 * It manages the i18n itself, so field name should be in it's english form.
 *
 * To use, you essentially give it the reference to a MailCommon::SearchRule and
 * it does the rest. It will never delete the rule itself, as it assumes
 *  that something outside of it manages this.
 *
 * @short A widget to edit a single MailCommon::SearchRule.
 * @author Marc Mutz <mutz@kde.org>
 */
class SearchRuleWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor. You can give a MailCommon::SearchRule as parameter,
     * which will be used to initialize the widget.
     */
    explicit SearchRuleWidget(QWidget *parent = nullptr,
                              MailCommon::SearchRule::Ptr aRule = MailCommon::SearchRule::Ptr(),
                              SearchPatternEdit::SearchPatternEditOptions options = (SearchPatternEdit::SearchPatternEditOptions)(SearchPatternEdit::None),
                              SearchPatternEdit::SearchModeType modeType = SearchPatternEdit::StandardMode);

    enum { Message, Body, AnyHeader, Recipients, Size, AgeInDays, Status, Tag, Subject, From, To, CC, ReplyTo, Organization, Date, Encryption };

    /**
     * Sets the rule. The rule is accepted regardless of the return
     * value of MailCommon::SearchRule::isEmpty. This widget makes a shallow
     * copy of @p aRule and operates directly on it. If @p aRule is 0,
     * resets itself, taks user input, but does essentially nothing.
     * If you pass 0, you should probably disable it.
     */
    void setRule(MailCommon::SearchRule::Ptr aRule);

    /**
     * Returns a reference to the currently-worked-on MailCommon::SearchRule.
     */
    MailCommon::SearchRule::Ptr rule() const;

    /**
     * Resets the rule currently worked on and updates the widget accordingly.
     */
    void reset();

    static int ruleFieldToId(const QString &i18nVal);

    void updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled);

    void setPatternEditOptions(MailCommon::SearchPatternEdit::SearchPatternEditOptions options);

public Q_SLOTS:
    void slotFunctionChanged();
    void slotValueChanged();
    void slotReturnPressed();

Q_SIGNALS:
    /**
     * This signal is emitted whenever the user alters the field.
     * The pseudo-headers <...> are returned in their i18n form, but
     * stored in their English form in the rule.
     */
    void fieldChanged(const QString &);

    /**
     * This signal is emitted whenever the user alters the contents/value
     * of the rule.
     */
    void contentsChanged(const QString &);

    void returnPressed();

    void addWidget(QWidget *);
    void removeWidget(QWidget *);

protected:
    /**
     * Used internally to translate i18n-ized pseudo-headers back to English.
     */
    static QByteArray ruleFieldToEnglish(const QString &i18nVal);

    /**
     * Used internally to find the corresponding index into the field
     * ComboBox. Returns the index if found or -1 if the search failed,
     */
    int indexOfRuleField(const QByteArray &aName) const;

protected Q_SLOTS:
    void slotRuleFieldChanged(const QString &);
    void slotAddWidget();
    void slotRemoveWidget();

private:
    void initWidget(SearchPatternEdit::SearchModeType modeType);
    void initFieldList(MailCommon::SearchPatternEdit::SearchPatternEditOptions options);

    QStringList mFilterFieldList;
    KComboBox *mRuleField = nullptr;
    QStackedWidget *mFunctionStack = nullptr;
    QStackedWidget *mValueStack = nullptr;
    QPushButton *mAdd = nullptr;
    QPushButton *mRemove = nullptr;
};

class SearchRuleWidgetLister : public KPIM::KWidgetLister
{
    Q_OBJECT

    friend class SearchPatternEdit;

public:
    explicit SearchRuleWidgetLister(QWidget *parent = nullptr,
                                    SearchPatternEdit::SearchPatternEditOptions opt = (SearchPatternEdit::SearchPatternEditOptions)(SearchPatternEdit::None),
                                    SearchPatternEdit::SearchModeType modeType = SearchPatternEdit::StandardMode);

    ~SearchRuleWidgetLister() override;

    void setRuleList(QList<MailCommon::SearchRule::Ptr> *aList);

    void setPatternEditOptions(SearchPatternEdit::SearchPatternEditOptions options);

public Q_SLOTS:
    void reset();
    void slotAddWidget(QWidget *);
    void slotRemoveWidget(QWidget *);

protected:
    void clearWidget(QWidget *aWidget) override;
    QWidget *createWidget(QWidget *parent) override;

private:
    void reconnectWidget(SearchRuleWidget *w);
    void updateAddRemoveButton();
    void regenerateRuleListFromWidgets();
    QList<MailCommon::SearchRule::Ptr> *mRuleList = nullptr;
    SearchPatternEdit::SearchPatternEditOptions mOptions;
    SearchPatternEdit::SearchModeType mTypeMode;
};
}

#endif

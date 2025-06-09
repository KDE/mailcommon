/*

  SPDX-FileCopyrightText: Marc Mutz <mutz@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "searchpatternedit.h"
#include "widgethandler/rulewidgethandlermanager.h"
using MailCommon::RuleWidgetHandlerManager;
#include "mailcommon_debug.h"

#include <KComboBox>
#include <KLazyLocalizedString>
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QButtonGroup>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QStackedWidget>
// Definition of special rule field strings
// Note: Also see SearchRule::matches() and ruleFieldToEnglish() if
//       you change the following i18n-ized strings!
// Note: The index of the values in the following array has to correspond to
//       the value of the entries in the enum in SearchRuleWidget.

using namespace MailCommon;

static const struct {
    const char *internalName;
    const KLazyLocalizedString displayName;

    [[nodiscard]] QString getLocalizedDisplayName() const
    {
        return displayName.toString();
    }
} SpecialRuleFields[] = {{"<message>", kli18n("Complete Message")},
                         {"<body>", kli18n("Body of Message")},
                         {"<any header>", kli18n("Anywhere in Headers")},
                         {"<recipients>", kli18n("All Recipients")},
                         {"<size>", kli18n("Size in Bytes")},
                         {"<age in days>", kli18n("Age in Days")},
                         {"<status>", kli18n("Message Status")},
                         {"<tag>", kli18n("Message Tag")},
                         {"Subject", kli18n("Subject")},
                         {"From", kli18n("From")},
                         {"To", kli18n("To")},
                         {"CC", kli18n("CC")},
                         {"Reply-To", kli18n("Reply To")},
                         {"Organization", kli18n("Organization")},
                         {"<date>", kli18n("Date")},
                         {"<encryption>", kli18n("Encryption")},
                         {"<attachment>", kli18n("Attachment")},
                         {"<invitation>", kli18n("Invitation")}};
static const int SpecialRuleFieldsCount = sizeof(SpecialRuleFields) / sizeof(*SpecialRuleFields);

//=============================================================================
//
// class SearchRuleWidget
//
//=============================================================================

SearchRuleWidget::SearchRuleWidget(QWidget *parent,
                                   SearchRule::Ptr aRule,
                                   SearchPatternEdit::SearchPatternEditOptions options,
                                   SearchPatternEdit::SearchModeType modeType)
    : QWidget(parent)
{
    initFieldList(options);
    initWidget(modeType);

    if (aRule) {
        setRule(aRule);
    } else {
        reset();
    }
}

void SearchRuleWidget::setPatternEditOptions(SearchPatternEdit::SearchPatternEditOptions options)
{
    SearchRule::Ptr srule = rule();
    QByteArray currentText = srule->field();

    initFieldList(options);

    mRuleField->clear();
    mRuleField->addItems(mFilterFieldList);
    KCompletion *comp = mRuleField->completionObject();
    comp->clear();
    comp->insertItems(mFilterFieldList);
    mRuleField->setMaxCount(mRuleField->count());
    mRuleField->adjustSize();

    const bool headersOnly = (options & MailCommon::SearchPatternEdit::HeadersOnly);
    const bool notShowSize = (options & MailCommon::SearchPatternEdit::NotShowSize);
    const bool notShowDate = (options & MailCommon::SearchPatternEdit::NotShowDate);
    const bool notShowAbsoluteDates = (options & MailCommon::SearchPatternEdit::NotShowAbsoluteDate);
    const bool notShowTags = (options & MailCommon::SearchPatternEdit::NotShowTags);

    if (headersOnly && (currentText != "<message>") && (currentText != "<body>")) {
        mRuleField->setItemText(0, QString::fromLatin1(currentText));
    } else {
        mRuleField->setItemText(0, QString());
    }

    if (notShowSize && (currentText != "<size>")) {
        mRuleField->setItemText(0, QString::fromLatin1(currentText));
    } else {
        mRuleField->setItemText(0, QString());
    }

    if (notShowDate && (currentText != "<date>")) {
        mRuleField->setItemText(0, QString::fromLatin1(currentText));
    } else {
        mRuleField->setItemText(0, QString());
    }

    if (notShowAbsoluteDates && (currentText != "<age in days>")) {
        mRuleField->setItemText(0, QString::fromLatin1(currentText));
    } else {
        mRuleField->setItemText(0, QString());
    }
    if (notShowTags && (currentText != "<tag>")) {
        mRuleField->setItemText(0, QString::fromLatin1(currentText));
    } else {
        mRuleField->setItemText(0, QString());
    }
}

void SearchRuleWidget::initWidget(SearchPatternEdit::SearchModeType modeType)
{
    auto hlay = new QHBoxLayout(this);
    hlay->setContentsMargins({});

    // initialize the header field combo box
    mRuleField = new KComboBox(this);
    mRuleField->setMinimumWidth(50);
    mRuleField->setObjectName(QLatin1StringView("mRuleField"));
    mRuleField->setEditable(true);
    auto edit = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(edit);
    edit->setPlaceholderText(i18nc("@info:placeholder", "Choose or type your own criteria"));
    mRuleField->setToolTip(i18nc("@info:tooltip", "Choose or type your own criteria"));
    edit->setClearButtonEnabled(true);
    mRuleField->setLineEdit(edit);

    mRuleField->addItems(mFilterFieldList);
    KCompletion *comp = mRuleField->completionObject();
    comp->setIgnoreCase(true);
    comp->insertItems(mFilterFieldList);
    comp->setCompletionMode(KCompletion::CompletionPopupAuto);

    // don't show sliders when popping up this menu
    mRuleField->setMaxCount(mRuleField->count());
    mRuleField->adjustSize();
    hlay->addWidget(mRuleField);

    // initialize the function/value widget stack
    mFunctionStack = new QStackedWidget(this);
    // Don't expand the widget in vertical direction
    mFunctionStack->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    hlay->addWidget(mFunctionStack);

    mValueStack = new QStackedWidget(this);
    hlay->addWidget(mValueStack);
    hlay->setStretchFactor(mValueStack, 10);

    mAdd = new QPushButton(this);
    mAdd->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    mAdd->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    hlay->addWidget(mAdd);

    mRemove = new QPushButton(this);
    mRemove->setIcon(QIcon::fromTheme(QStringLiteral("list-remove")));
    mRemove->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    hlay->addWidget(mRemove);

    RuleWidgetHandlerManager::instance()->setIsAkonadiSearch(modeType == SearchPatternEdit::BalooMode);

    RuleWidgetHandlerManager::instance()->createWidgets(mFunctionStack, mValueStack, this);

    // redirect focus to the header field combo box
    setFocusProxy(mRuleField);

    connect(mRuleField, &QComboBox::textActivated, this, &SearchRuleWidget::slotRuleFieldChanged);
    connect(mRuleField, &QComboBox::editTextChanged, this, &SearchRuleWidget::slotRuleFieldChanged);
    connect(mRuleField, &QComboBox::editTextChanged, this, &SearchRuleWidget::fieldChanged);

    connect(mAdd, &QAbstractButton::clicked, this, &SearchRuleWidget::slotAddWidget);
    connect(mRemove, &QAbstractButton::clicked, this, &SearchRuleWidget::slotRemoveWidget);
}

void SearchRuleWidget::updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled)
{
    mAdd->setEnabled(addButtonEnabled);
    mRemove->setEnabled(removeButtonEnabled);
}

void SearchRuleWidget::slotAddWidget()
{
    Q_EMIT addWidget(this);
}

void SearchRuleWidget::slotRemoveWidget()
{
    Q_EMIT removeWidget(this);
}

void SearchRuleWidget::setRule(SearchRule::Ptr aRule)
{
    Q_ASSERT(aRule);

    qCDebug(MAILCOMMON_LOG) << "(" << aRule->asString() << ")";

    //--------------set the field
    int i = indexOfRuleField(aRule->field());

    mRuleField->blockSignals(true);

    if (i < 0) { // not found -> user defined field
        mRuleField->setItemText(0, QString::fromLatin1(aRule->field()));
        i = 0;
    } else { // found in the list of predefined fields
        mRuleField->setItemText(0, QString());
    }

    mRuleField->setCurrentIndex(i);
    mRuleField->blockSignals(false);

    RuleWidgetHandlerManager::instance()->setRule(mFunctionStack, mValueStack, aRule);
}

SearchRule::Ptr SearchRuleWidget::rule() const
{
    const QByteArray ruleField = ruleFieldToEnglish(mRuleField->currentText());

    const SearchRule::Function function = RuleWidgetHandlerManager::instance()->function(ruleField, mFunctionStack);

    const QString value = RuleWidgetHandlerManager::instance()->value(ruleField, mFunctionStack, mValueStack);

    return SearchRule::createInstance(ruleField, function, value);
}

void SearchRuleWidget::reset()
{
    mRuleField->blockSignals(true);
    mRuleField->setItemText(0, QString());
    mRuleField->setCurrentIndex(0);
    mRuleField->blockSignals(false);

    RuleWidgetHandlerManager::instance()->reset(mFunctionStack, mValueStack);
}

void SearchRuleWidget::slotFunctionChanged()
{
    const QByteArray ruleField = ruleFieldToEnglish(mRuleField->currentText());
    RuleWidgetHandlerManager::instance()->update(ruleField, mFunctionStack, mValueStack);
    const QString prettyValue = RuleWidgetHandlerManager::instance()->prettyValue(ruleField, mFunctionStack, mValueStack);

    Q_EMIT contentsChanged(prettyValue);
}

void SearchRuleWidget::slotValueChanged()
{
    const QByteArray ruleField = ruleFieldToEnglish(mRuleField->currentText());

    const QString prettyValue = RuleWidgetHandlerManager::instance()->prettyValue(ruleField, mFunctionStack, mValueStack);

    Q_EMIT contentsChanged(prettyValue);
}

void SearchRuleWidget::slotReturnPressed()
{
    Q_EMIT returnPressed();
}

QByteArray SearchRuleWidget::ruleFieldToEnglish(const QString &i18nVal)
{
    for (int i = 0; i < SpecialRuleFieldsCount; ++i) {
        if (i18nVal == SpecialRuleFields[i].getLocalizedDisplayName()) {
            return SpecialRuleFields[i].internalName;
        }
    }
    return i18nVal.toLatin1();
}

int SearchRuleWidget::ruleFieldToId(const QString &i18nVal)
{
    for (int i = 0; i < SpecialRuleFieldsCount; ++i) {
        if (i18nVal == SpecialRuleFields[i].getLocalizedDisplayName()) {
            return i;
        }
    }
    return -1; // no pseudo header
}

static QString displayNameFromInternalName(const QString &internal)
{
    for (int i = 0; i < SpecialRuleFieldsCount; ++i) {
        if (internal == QLatin1StringView(SpecialRuleFields[i].internalName)) {
            return SpecialRuleFields[i].getLocalizedDisplayName();
        }
    }
    return QLatin1StringView(internal.toLatin1());
}

int SearchRuleWidget::indexOfRuleField(const QByteArray &aName) const
{
    if (aName.isEmpty()) {
        return -1;
    }

    const QString i18n_aName = displayNameFromInternalName(QLatin1StringView(aName));
    const int nbRuleField = mRuleField->count();
    for (int i = 1; i < nbRuleField; ++i) {
        if (mRuleField->itemText(i) == i18n_aName) {
            return i;
        }
    }

    return -1;
}

void SearchRuleWidget::initFieldList(SearchPatternEdit::SearchPatternEditOptions options)
{
    const bool headersOnly = (options & MailCommon::SearchPatternEdit::HeadersOnly);
    const bool notShowAbsoluteDates = (options & MailCommon::SearchPatternEdit::NotShowAbsoluteDate);
    const bool notShowSize = (options & MailCommon::SearchPatternEdit::NotShowSize);
    const bool notShowDate = (options & MailCommon::SearchPatternEdit::NotShowDate);
    const bool notShowTags = (options & MailCommon::SearchPatternEdit::NotShowTags);

    mFilterFieldList.clear();
    mFilterFieldList.append(QString()); // empty entry for user input

    if (!headersOnly) {
        mFilterFieldList.append(SpecialRuleFields[Message].getLocalizedDisplayName());
        mFilterFieldList.append(SpecialRuleFields[Body].getLocalizedDisplayName());
    }
    mFilterFieldList.append(SpecialRuleFields[AnyHeader].getLocalizedDisplayName());
    mFilterFieldList.append(SpecialRuleFields[Recipients].getLocalizedDisplayName());
    if (!notShowSize) {
        mFilterFieldList.append(SpecialRuleFields[Size].getLocalizedDisplayName());
    }
    if (!notShowAbsoluteDates) {
        mFilterFieldList.append(SpecialRuleFields[AgeInDays].getLocalizedDisplayName());
    }

    mFilterFieldList.append(SpecialRuleFields[Subject].getLocalizedDisplayName());
    mFilterFieldList.append(SpecialRuleFields[From].getLocalizedDisplayName());
    mFilterFieldList.append(SpecialRuleFields[To].getLocalizedDisplayName());
    mFilterFieldList.append(SpecialRuleFields[CC].getLocalizedDisplayName());
    mFilterFieldList.append(SpecialRuleFields[Status].getLocalizedDisplayName());
    if (!notShowTags) {
        mFilterFieldList.append(SpecialRuleFields[Tag].getLocalizedDisplayName());
    }
    mFilterFieldList.append(SpecialRuleFields[ReplyTo].getLocalizedDisplayName());
    mFilterFieldList.append(SpecialRuleFields[Organization].getLocalizedDisplayName());

    if (!notShowDate) {
        mFilterFieldList.append(SpecialRuleFields[Date].getLocalizedDisplayName());
    }

    mFilterFieldList.append(SpecialRuleFields[Encryption].getLocalizedDisplayName());
    mFilterFieldList.append(SpecialRuleFields[Attachment].getLocalizedDisplayName());
    mFilterFieldList.append(SpecialRuleFields[Invitation].getLocalizedDisplayName());

    // these others only represent message headers and you can add to
    // them as you like
    mFilterFieldList.append(QStringLiteral("List-Id"));
    mFilterFieldList.append(QStringLiteral("Resent-From"));
    mFilterFieldList.append(QStringLiteral("X-Loop"));
    mFilterFieldList.append(QStringLiteral("X-Mailing-List"));
    mFilterFieldList.append(QStringLiteral("X-Spam-Flag"));
    mFilterFieldList.append(QStringLiteral("X-Spam-Status"));
}

void SearchRuleWidget::slotRuleFieldChanged(const QString &field)
{
    RuleWidgetHandlerManager::instance()->update(ruleFieldToEnglish(field), mFunctionStack, mValueStack);
}

//=============================================================================
//
// class KMFilterActionWidgetLister (the filter action editor)
//
//=============================================================================

SearchRuleWidgetLister::SearchRuleWidgetLister(QWidget *parent, SearchPatternEdit::SearchPatternEditOptions options, SearchPatternEdit::SearchModeType modeType)
    : KWidgetLister(false, 1, SearchPattern::filterRulesMaximumSize(), parent)
{
    mRuleList = nullptr;

    mTypeMode = modeType;
    mOptions = options;
}

SearchRuleWidgetLister::~SearchRuleWidgetLister() = default;

void SearchRuleWidgetLister::setPatternEditOptions(SearchPatternEdit::SearchPatternEditOptions options)
{
    mOptions = options;
    const auto lst = widgets();
    for (QWidget *w : lst) {
        qobject_cast<SearchRuleWidget *>(w)->setPatternEditOptions(options);
    }
}

void SearchRuleWidgetLister::setRuleList(QList<SearchRule::Ptr> *aList)
{
    Q_ASSERT(aList);

    if (mRuleList && mRuleList != aList) {
        regenerateRuleListFromWidgets();
    }

    mRuleList = aList;

    if (!widgets().isEmpty()) { // move this below next 'if'?
        widgets().constFirst()->blockSignals(true);
    }

    if (aList->isEmpty()) {
        slotClear();
        widgets().constFirst()->blockSignals(false);
        return;
    }

    int superfluousItems = (int)mRuleList->count() - widgetsMaximum();
    if (superfluousItems > 0) {
        qCDebug(MAILCOMMON_LOG) << "Clipping rule list to" << widgetsMaximum() << "items!";

        for (; superfluousItems; superfluousItems--) {
            mRuleList->removeLast();
        }
    }

    // set the right number of widgets
    setNumberOfShownWidgetsTo(qMax((int)mRuleList->count(), widgetsMinimum()));

    // load the actions into the widgets
    QList<QWidget *> widgetList = widgets();
    QList<SearchRule::Ptr>::const_iterator rIt;
    QList<SearchRule::Ptr>::const_iterator rItEnd(mRuleList->constEnd());
    QList<QWidget *>::const_iterator with = widgetList.constBegin();
    QList<QWidget *>::const_iterator wItEnd = widgetList.constEnd();
    for (rIt = mRuleList->constBegin(); rIt != rItEnd && with != wItEnd; ++rIt, ++with) {
        qobject_cast<SearchRuleWidget *>(*with)->setRule((*rIt));
    }
    for (; with != wItEnd; ++with) {
        qobject_cast<SearchRuleWidget *>(*with)->reset();
    }

    Q_ASSERT(!widgets().isEmpty());
    widgets().constFirst()->blockSignals(false);
    updateAddRemoveButton();
}

void SearchRuleWidgetLister::slotAddWidget(QWidget *w)
{
    addWidgetAfterThisWidget(w);
    updateAddRemoveButton();
}

void SearchRuleWidgetLister::slotRemoveWidget(QWidget *w)
{
    removeWidget(w);
    updateAddRemoveButton();
}

void SearchRuleWidgetLister::reconnectWidget(SearchRuleWidget *w)
{
    connect(w, &SearchRuleWidget::addWidget, this, &SearchRuleWidgetLister::slotAddWidget, Qt::UniqueConnection);
    connect(w, &SearchRuleWidget::removeWidget, this, &SearchRuleWidgetLister::slotRemoveWidget, Qt::UniqueConnection);
}

void SearchRuleWidgetLister::updateAddRemoveButton()
{
    QList<QWidget *> widgetList = widgets();
    const int numberOfWidget(widgetList.count());
    bool addButtonEnabled = false;
    bool removeButtonEnabled = false;
    if (numberOfWidget <= widgetsMinimum()) {
        addButtonEnabled = true;
        removeButtonEnabled = false;
    } else if (numberOfWidget >= widgetsMaximum()) {
        addButtonEnabled = false;
        removeButtonEnabled = true;
    } else {
        addButtonEnabled = true;
        removeButtonEnabled = true;
    }
    QList<QWidget *>::ConstIterator with = widgetList.constBegin();
    QList<QWidget *>::ConstIterator wEnd = widgetList.constEnd();
    for (; with != wEnd; ++with) {
        auto w = qobject_cast<SearchRuleWidget *>(*with);
        w->updateAddRemoveButton(addButtonEnabled, removeButtonEnabled);
    }
}

void SearchRuleWidgetLister::reset()
{
    if (mRuleList) {
        regenerateRuleListFromWidgets();
    }

    mRuleList = nullptr;
    slotClear();
    updateAddRemoveButton();
}

QWidget *SearchRuleWidgetLister::createWidget(QWidget *parent)
{
    auto w = new SearchRuleWidget(parent, SearchRule::Ptr(), mOptions, mTypeMode);
    reconnectWidget(w);
    return w;
}

void SearchRuleWidgetLister::clearWidget(QWidget *aWidget)
{
    if (aWidget) {
        auto w = static_cast<SearchRuleWidget *>(aWidget);
        w->reset();
        reconnectWidget(w);
        updateAddRemoveButton();
    }
}

void SearchRuleWidgetLister::regenerateRuleListFromWidgets()
{
    if (!mRuleList) {
        return;
    }

    mRuleList->clear();

    const auto lst = widgets();
    for (const QWidget *w : lst) {
        SearchRule::Ptr r = qobject_cast<const SearchRuleWidget *>(w)->rule();
        if (r && !r->isEmpty()) {
            mRuleList->append(r);
        }
    }
    updateAddRemoveButton();
}

//=============================================================================
//
// class SearchPatternEdit
//
//=============================================================================

SearchPatternEdit::SearchPatternEdit(QWidget *parent, SearchPatternEditOptions options, SearchModeType modeType)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setObjectName(QLatin1StringView("SearchPatternEdit"));
    initLayout(options, modeType);
}

SearchPatternEdit::~SearchPatternEdit() = default;

void SearchPatternEdit::updateSearchPattern()
{
    mRuleLister->regenerateRuleListFromWidgets();
}

void SearchPatternEdit::setPatternEditOptions(SearchPatternEdit::SearchPatternEditOptions options)
{
    mRuleLister->setPatternEditOptions(options);
}

void SearchPatternEdit::initLayout(SearchPatternEditOptions options, SearchModeType modeType)
{
    auto layout = new QFormLayout(this);
    layout->setContentsMargins({});

    const bool matchAllMessages = (options & MailCommon::SearchPatternEdit::MatchAllMessages);
    //------------the radio buttons
    mAllRBtn = new QRadioButton(i18nc("@option:radio", "Match a&ll of the following"), this);
    mAnyRBtn = new QRadioButton(i18nc("@option:radio", "Match an&y of the following"), this);
    if (matchAllMessages) {
        mAllMessageRBtn = new QRadioButton(i18nc("@option:radio", "Match all messages"), this);
    }

    mAllRBtn->setObjectName(QLatin1StringView("mAllRBtn"));
    mAllRBtn->setChecked(true);
    mAnyRBtn->setObjectName(QLatin1StringView("mAnyRBtn"));
    mAnyRBtn->setChecked(false);
    if (matchAllMessages) {
        mAllMessageRBtn->setObjectName(QLatin1StringView("mAllMessageRBtn"));
        mAllMessageRBtn->setChecked(false);
    }
    layout->addRow(i18n("Filter criteria:"), mAllRBtn);
    layout->addRow(QString(), mAnyRBtn);
    if (matchAllMessages) {
        layout->addRow(QString(), mAllMessageRBtn);
    }

    auto bg = new QButtonGroup(this);
    bg->addButton(mAllRBtn);
    bg->addButton(mAnyRBtn);
    if (matchAllMessages) {
        bg->addButton(mAllMessageRBtn);
    }

    //------------connect a few signals
    connect(bg, &QButtonGroup::buttonClicked, this, &SearchPatternEdit::slotRadioClicked);

    //------------the list of SearchRuleWidget's
    mRuleLister = new SearchRuleWidgetLister(this, options, modeType);

    mRuleLister->slotClear();

    if (!mRuleLister->widgets().isEmpty()) {
        const int numberOfWidget(mRuleLister->widgets().count());
        for (int i = 0; i < numberOfWidget; ++i) {
            SearchRuleWidget *srw = static_cast<SearchRuleWidget *>(mRuleLister->widgets().at(i));
            connect(srw, &SearchRuleWidget::fieldChanged, this, &SearchPatternEdit::slotAutoNameHack);
            connect(srw, &SearchRuleWidget::contentsChanged, this, &SearchPatternEdit::slotAutoNameHack);
            connect(srw, &SearchRuleWidget::returnPressed, this, &SearchPatternEdit::returnPressed);
        }
    } else {
        qCDebug(MAILCOMMON_LOG) << "No first SearchRuleWidget, though slotClear() has been called!";
    }

    connect(mRuleLister, qOverload<QWidget *>(&SearchRuleWidgetLister::widgetAdded), this, &SearchPatternEdit::slotRuleAdded);
    connect(mRuleLister, qOverload<>(&SearchRuleWidgetLister::widgetRemoved), this, &SearchPatternEdit::patternChanged);
    connect(mRuleLister, &KPIM::KWidgetLister::clearWidgets, this, &SearchPatternEdit::patternChanged);

    layout->addRow(mRuleLister);
}

void SearchPatternEdit::setSearchPattern(SearchPattern *aPattern)
{
    Q_ASSERT(aPattern);

    mRuleLister->setRuleList(aPattern);

    mPattern = aPattern;

    blockSignals(true);
    if (mPattern->op() == SearchPattern::OpOr) {
        mAnyRBtn->setChecked(true);
    } else if (mPattern->op() == SearchPattern::OpAnd) {
        mAllRBtn->setChecked(true);
    } else if (mAllMessageRBtn && (mPattern->op() == SearchPattern::OpAll)) {
        mAllMessageRBtn->setChecked(true);
    }
    mRuleLister->setEnabled(mPattern->op() != SearchPattern::OpAll);
    blockSignals(false);

    setEnabled(true);
    Q_EMIT patternChanged();
}

void SearchPatternEdit::reset()
{
    mRuleLister->reset();

    blockSignals(true);
    mAllRBtn->setChecked(true);
    blockSignals(false);

    setEnabled(false);
    Q_EMIT patternChanged();
}

void SearchPatternEdit::slotRadioClicked(QAbstractButton *aRBtn)
{
    if (mPattern) {
        if (aRBtn == mAllRBtn) {
            mPattern->setOp(SearchPattern::OpAnd);
        } else if (aRBtn == mAnyRBtn) {
            mPattern->setOp(SearchPattern::OpOr);
        } else if (aRBtn == mAllMessageRBtn) {
            mPattern->setOp(SearchPattern::OpAll);
        }
        mRuleLister->setEnabled(mPattern->op() != SearchPattern::OpAll);
        Q_EMIT patternChanged();
    }
}

void SearchPatternEdit::slotAutoNameHack()
{
    mRuleLister->regenerateRuleListFromWidgets();
    Q_EMIT maybeNameChanged();
    Q_EMIT patternChanged();
}

void SearchPatternEdit::slotRuleAdded(QWidget *newRuleWidget)
{
    auto srw = static_cast<SearchRuleWidget *>(newRuleWidget);
    connect(srw, &SearchRuleWidget::fieldChanged, this, &SearchPatternEdit::slotAutoNameHack);
    connect(srw, &SearchRuleWidget::contentsChanged, this, &SearchPatternEdit::slotAutoNameHack);
    connect(srw, &SearchRuleWidget::returnPressed, this, &SearchPatternEdit::returnPressed);
    Q_EMIT patternChanged();
}

#include "moc_searchpatternedit.cpp"

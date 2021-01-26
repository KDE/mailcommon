/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "tagrulewidgethandler.h"
#include "mailcommon_debug.h"
#include "search/searchpattern.h"

#include <KJob>
#include <KLineEdit>
#include <KLocalizedString>
#include <QIcon>

#include <Tag>
#include <TagAttribute>
#include <TagFetchJob>
#include <TagFetchScope>

#include <QComboBox>
#include <QLineEdit>
#include <QStackedWidget>

using namespace MailCommon;

class FillTagComboJob : public KJob
{
    Q_OBJECT
public:
    explicit FillTagComboJob(QComboBox *combo, QObject *parent = nullptr);
    void start() override;
private Q_SLOTS:
    void onDestroyed();
    void onTagsFetched(KJob *);

private:
    QComboBox *mComboBox = nullptr;
};

FillTagComboJob::FillTagComboJob(QComboBox *combo, QObject *parent)
    : KJob(parent)
    , mComboBox(combo)
{
    connect(combo, &QObject::destroyed, this, &FillTagComboJob::onDestroyed);
}

void FillTagComboJob::onDestroyed()
{
    mComboBox = nullptr;
    setError(KJob::UserDefinedError);
    qCDebug(MAILCOMMON_LOG) << "Combobox destroyed";
    emitResult();
}

void FillTagComboJob::start()
{
    auto fetchJob = new Akonadi::TagFetchJob(this);
    fetchJob->fetchScope().fetchAttribute<Akonadi::TagAttribute>();
    connect(fetchJob, &Akonadi::TagFetchJob::result, this, &FillTagComboJob::onTagsFetched);
}

void FillTagComboJob::onTagsFetched(KJob *job)
{
    if (job->error()) {
        qCWarning(MAILCOMMON_LOG) << job->errorString();
        setError(KJob::UserDefinedError);
        emitResult();
    }
    if (!mComboBox) {
        qCDebug(MAILCOMMON_LOG) << "combobox already destroyed";
        emitResult();
        return;
    }
    auto fetchJob = static_cast<Akonadi::TagFetchJob *>(job);
    const auto lst = fetchJob->tags();
    for (const Akonadi::Tag &tag : lst) {
        QString iconName = QStringLiteral("mail-tagged");
        const auto *attr = tag.attribute<Akonadi::TagAttribute>();
        if (attr) {
            if (!attr->iconName().isEmpty()) {
                iconName = attr->iconName();
            }
        }
        mComboBox->addItem(QIcon::fromTheme(iconName), tag.name(), tag.url().url());
    }
    emitResult();
}

static const struct {
    SearchRule::Function id;
    const char *displayName;
} TagFunctions[] = {{SearchRule::FuncContains, I18N_NOOP("contains")},
                    {SearchRule::FuncContainsNot, I18N_NOOP("does not contain")},
                    {SearchRule::FuncEquals, I18N_NOOP("equals")},
                    {SearchRule::FuncNotEqual, I18N_NOOP("does not equal")},
                    {SearchRule::FuncRegExp, I18N_NOOP("matches regular expr.")},
                    {SearchRule::FuncNotRegExp, I18N_NOOP("does not match reg. expr.")}};
static const int TagFunctionCount = sizeof(TagFunctions) / sizeof(*TagFunctions);

//---------------------------------------------------------------------------

QWidget *TagRuleWidgetHandler::createFunctionWidget(int number, QStackedWidget *functionStack, const QObject *receiver, bool isBalooSearch) const
{
    if (number != 0) {
        return nullptr;
    }

    const auto funcCombo = new QComboBox(functionStack);
    funcCombo->setMinimumWidth(50);
    funcCombo->setObjectName(QStringLiteral("tagRuleFuncCombo"));
    for (int i = 0; i < TagFunctionCount; ++i) {
        if (isBalooSearch) {
            if (TagFunctions[i].id == SearchRule::FuncContains || TagFunctions[i].id == SearchRule::FuncContainsNot) {
                funcCombo->addItem(i18n(TagFunctions[i].displayName));
            }
        } else {
            funcCombo->addItem(i18n(TagFunctions[i].displayName));
        }
    }
    funcCombo->adjustSize();
    QObject::connect(funcCombo, SIGNAL(activated(int)), receiver, SLOT(slotFunctionChanged()));
    return funcCombo;
}

//---------------------------------------------------------------------------

QWidget *TagRuleWidgetHandler::createValueWidget(int number, QStackedWidget *valueStack, const QObject *receiver) const
{
    if (number == 0) {
        auto lineEdit = new KLineEdit(valueStack);
        lineEdit->setClearButtonEnabled(true);
        lineEdit->setTrapReturnKey(true);
        lineEdit->setObjectName(QStringLiteral("tagRuleRegExpLineEdit"));
        QObject::connect(lineEdit, SIGNAL(textChanged(QString)), receiver, SLOT(slotValueChanged()));
        QObject::connect(lineEdit, SIGNAL(returnPressed()), receiver, SLOT(slotReturnPressed()));
        return lineEdit;
    }

    if (number == 1) {
        const auto valueCombo = new QComboBox(valueStack);
        valueCombo->setMinimumWidth(50);
        valueCombo->setObjectName(QStringLiteral("tagRuleValueCombo"));
        valueCombo->setEditable(true);
        valueCombo->addItem(QString()); // empty entry for user input

        auto job = new FillTagComboJob(valueCombo);
        job->start();

        valueCombo->adjustSize();
        QObject::connect(valueCombo, SIGNAL(activated(int)), receiver, SLOT(slotValueChanged()));
        return valueCombo;
    }

    return nullptr;
}

//---------------------------------------------------------------------------

SearchRule::Function TagRuleWidgetHandler::function(const QByteArray &field, const QStackedWidget *functionStack) const
{
    if (!handlesField(field)) {
        return SearchRule::FuncNone;
    }

    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("tagRuleFuncCombo"));

    if (funcCombo && funcCombo->currentIndex() >= 0) {
        return TagFunctions[funcCombo->currentIndex()].id;
    }
    return SearchRule::FuncNone;
}

//---------------------------------------------------------------------------

QString TagRuleWidgetHandler::value(const QByteArray &field, const QStackedWidget *functionStack, const QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return QString();
    }

    SearchRule::Function func = function(field, functionStack);
    if (func == SearchRule::FuncRegExp || func == SearchRule::FuncNotRegExp) {
        // Use regexp line edit
        const KLineEdit *lineEdit = valueStack->findChild<KLineEdit *>(QStringLiteral("tagRuleRegExpLineEdit"));

        if (lineEdit) {
            return lineEdit->text();
        } else {
            return QString();
        }
    }

    // Use combo box
    const auto tagCombo = valueStack->findChild<QComboBox *>(QStringLiteral("tagRuleValueCombo"));

    if (tagCombo) {
        return tagCombo->itemData(tagCombo->currentIndex()).toString();
    } else {
        return QString();
    }
}

//---------------------------------------------------------------------------

QString TagRuleWidgetHandler::prettyValue(const QByteArray &field, const QStackedWidget *funcStack, const QStackedWidget *valueStack) const
{
    return value(field, funcStack, valueStack);
}

//---------------------------------------------------------------------------

bool TagRuleWidgetHandler::handlesField(const QByteArray &field) const
{
    return field == "<tag>";
}

//---------------------------------------------------------------------------

void TagRuleWidgetHandler::reset(QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    // reset the function combo box
    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("tagRuleFuncCombo"));

    if (funcCombo) {
        funcCombo->blockSignals(true);
        funcCombo->setCurrentIndex(0);
        funcCombo->blockSignals(false);
    }

    // reset the status value combo box and reg exp line edit
    auto *lineEdit = valueStack->findChild<KLineEdit *>(QStringLiteral("tagRuleRegExpLineEdit"));

    if (lineEdit) {
        lineEdit->blockSignals(true);
        lineEdit->clear();
        lineEdit->blockSignals(false);
        lineEdit->setClearButtonEnabled(false);
        lineEdit->setClearButtonEnabled(true);
        valueStack->setCurrentWidget(lineEdit);
    }

    const auto tagCombo = valueStack->findChild<QComboBox *>(QStringLiteral("tagRuleValueCombo"));
    if (tagCombo) {
        tagCombo->blockSignals(true);
        tagCombo->setCurrentIndex(0);
        tagCombo->blockSignals(false);
    }
}

//---------------------------------------------------------------------------

bool TagRuleWidgetHandler::setRule(QStackedWidget *functionStack, QStackedWidget *valueStack, const SearchRule::Ptr rule, bool isBalooSearch) const
{
    if (!rule || !handlesField(rule->field())) {
        reset(functionStack, valueStack);
        return false;
    }

    // set the function
    const SearchRule::Function func = rule->function();

    if (isBalooSearch) {
        if (func != SearchRule::FuncContains && func != SearchRule::FuncContainsNot) {
            reset(functionStack, valueStack);
            return false;
        }
    }

    int funcIndex = 0;
    for (; funcIndex < TagFunctionCount; ++funcIndex) {
        if (func == TagFunctions[funcIndex].id) {
            break;
        }
    }

    const auto funcCombo = functionStack->findChild<QComboBox *>(QStringLiteral("tagRuleFuncCombo"));

    if (funcCombo) {
        funcCombo->blockSignals(true);
        if (funcIndex < TagFunctionCount) {
            funcCombo->setCurrentIndex(funcIndex);
        } else {
            funcCombo->setCurrentIndex(0);
        }
        funcCombo->blockSignals(false);
        functionStack->setCurrentWidget(funcCombo);
    }

    // set the value
    if (func == SearchRule::FuncRegExp || func == SearchRule::FuncNotRegExp) {
        // set reg exp value
        auto *lineEdit = valueStack->findChild<KLineEdit *>(QStringLiteral("tagRuleRegExpLineEdit"));

        if (lineEdit) {
            lineEdit->blockSignals(true);
            lineEdit->setText(rule->contents());
            lineEdit->blockSignals(false);
            lineEdit->setClearButtonEnabled(false);
            lineEdit->setClearButtonEnabled(true);
            valueStack->setCurrentWidget(lineEdit);
        }
    } else {
        // set combo box value
        const auto tagCombo = valueStack->findChild<QComboBox *>(QStringLiteral("tagRuleValueCombo"));

        if (tagCombo) {
            tagCombo->blockSignals(true);
            bool found = false;
            // Existing tags numbered from 1
            for (int i = 1; i < tagCombo->count(); i++) {
                if (rule->contents() == tagCombo->itemData(i).toString()) {
                    tagCombo->setCurrentIndex(i);
                    found = true;
                    break;
                }
            }
            if (!found) {
                tagCombo->setCurrentIndex(0);
                // Still show tag if it was deleted from MsgTagMgr
                QLineEdit *lineEdit = tagCombo->lineEdit(); // krazy:exclude=qclasses
                Q_ASSERT(lineEdit);
                lineEdit->setText(rule->contents());
            }

            tagCombo->blockSignals(false);
            valueStack->setCurrentWidget(tagCombo);
        }
    }
    return true;
}

//---------------------------------------------------------------------------

bool TagRuleWidgetHandler::update(const QByteArray &field, QStackedWidget *functionStack, QStackedWidget *valueStack) const
{
    if (!handlesField(field)) {
        return false;
    }

    // raise the correct function widget
    functionStack->setCurrentWidget(functionStack->findChild<QWidget *>(QStringLiteral("tagRuleFuncCombo")));

    // raise the correct value widget
    SearchRule::Function func = function(field, functionStack);
    if (func == SearchRule::FuncRegExp || func == SearchRule::FuncNotRegExp) {
        valueStack->setCurrentWidget(valueStack->findChild<QWidget *>(QStringLiteral("tagRuleRegExpLineEdit")));
    } else {
        valueStack->setCurrentWidget(valueStack->findChild<QWidget *>(QStringLiteral("tagRuleValueCombo")));
    }

    return true;
}

#include "tagrulewidgethandler.moc"

/*
  SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  SPDX-FileContributor: Tobias Koenig <tokoe@kdab.com>

  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "snippetsmanager.h"
#include "mailcommon_debug.h"
#include "snippetdialog.h"
#include "snippetsmodel.h"
#include "snippetvariabledialog.h"
#include <KActionCollection>
#include <KSharedConfig>

#include <KLocalizedString>
#include <KMessageBox>
#include <QIcon>

#include <QAction>
#include <QItemSelectionModel>
#include <QPointer>
#include <QRegularExpression>

using namespace MailCommon;

class Q_DECL_HIDDEN SnippetsManager::Private
{
public:
    Private(SnippetsManager *qq, QWidget *parentWidget)
        : q(qq)
        , mParent(parentWidget)
    {
    }

    QModelIndex currentGroupIndex() const;

    void selectionChanged();
    void dndDone();
    void addSnippet();
    void editSnippet();
    void deleteSnippet();

    void addSnippetGroup();
    void editSnippetGroup();
    void deleteSnippetGroup();

    void insertSelectedSnippet();
    void insertActionSnippet();

    void createSnippet(const QString &text = QString());

    void slotAddNewDndSnippset(const QString &);

    void updateActionCollection(const QString &oldName,
                                const QString &newName,
                                const QKeySequence &keySequence,
                                const QString &text,
                                const QString &subject,
                                const QString &to,
                                const QString &cc,
                                const QString &bcc,
                                const QString &attachment);
    void initializeAction(const QString &newName,
                          const QKeySequence &keySequence,
                          const QString &text,
                          const QString &subject,
                          const QString &to,
                          const QString &cc,
                          const QString &bcc,
                          const QString &attachment);
    void initializeActionCollection();

    QString replaceVariables(const QString &text);

    void save();

    SnippetsManager *const q = nullptr;
    SnippetsModel *mModel = nullptr;
    QItemSelectionModel *mSelectionModel = nullptr;
    KActionCollection *mActionCollection = nullptr;
    QAction *mAddSnippetAction = nullptr;
    QAction *mEditSnippetAction = nullptr;
    QAction *mDeleteSnippetAction = nullptr;
    QAction *mAddSnippetGroupAction = nullptr;
    QAction *mEditSnippetGroupAction = nullptr;
    QAction *mDeleteSnippetGroupAction = nullptr;
    QAction *mInsertSnippetAction = nullptr;
    QWidget *mParent = nullptr;
    bool mDirty = false;
};

QModelIndex SnippetsManager::Private::currentGroupIndex() const
{
    if (mSelectionModel->selectedIndexes().isEmpty()) {
        return QModelIndex();
    }

    const QModelIndex index = mSelectionModel->selectedIndexes().first();
    if (index.data(SnippetsModel::IsGroupRole).toBool()) {
        return index;
    } else {
        return mModel->parent(index);
    }
}

void SnippetsManager::Private::selectionChanged()
{
    const bool itemSelected = !mSelectionModel->selectedIndexes().isEmpty();

    if (itemSelected) {
        const QModelIndex index = mSelectionModel->selectedIndexes().first();
        const bool isGroup = index.data(SnippetsModel::IsGroupRole).toBool();
        if (isGroup) {
            mEditSnippetAction->setEnabled(false);
            mDeleteSnippetAction->setEnabled(false);
            mEditSnippetGroupAction->setEnabled(true);
            mDeleteSnippetGroupAction->setEnabled(true);
            mInsertSnippetAction->setEnabled(false);
        } else {
            mEditSnippetAction->setEnabled(true);
            mDeleteSnippetAction->setEnabled(true);
            mEditSnippetGroupAction->setEnabled(false);
            mDeleteSnippetGroupAction->setEnabled(false);
            mInsertSnippetAction->setEnabled(true);
        }
    } else {
        mEditSnippetAction->setEnabled(false);
        mDeleteSnippetAction->setEnabled(false);
        mEditSnippetGroupAction->setEnabled(false);
        mDeleteSnippetGroupAction->setEnabled(false);
        mInsertSnippetAction->setEnabled(false);
    }
}

void SnippetsManager::Private::addSnippet()
{
    createSnippet();
}

void SnippetsManager::Private::createSnippet(const QString &text)
{
    const bool noGroupAvailable = (mModel->rowCount() == 0);

    if (noGroupAvailable) {
        // create a 'General' snippet group
        if (!mModel->insertRow(mModel->rowCount(), QModelIndex())) {
            return;
        }

        const QModelIndex groupIndex = mModel->index(mModel->rowCount() - 1, 0, QModelIndex());
        mModel->setData(groupIndex, i18n("General"), SnippetsModel::NameRole);

        mSelectionModel->select(groupIndex, QItemSelectionModel::ClearAndSelect);
    }

    QPointer<SnippetDialog> dlg = new SnippetDialog(mActionCollection, false, mParent);
    dlg->setWindowTitle(i18nc("@title:window", "Add Snippet"));
    dlg->setGroupModel(mModel);
    dlg->setGroupIndex(currentGroupIndex());
    dlg->setText(text);

    if (dlg->exec()) {
        const QModelIndex groupIndex = dlg->groupIndex();

        if (!mModel->insertRow(mModel->rowCount(groupIndex), groupIndex)) {
            delete dlg;
            return;
        }

        const QModelIndex index = mModel->index(mModel->rowCount(groupIndex) - 1, 0, groupIndex);
        mModel->setData(index, dlg->name(), SnippetsModel::NameRole);
        mModel->setData(index, dlg->text(), SnippetsModel::TextRole);
        mModel->setData(index, dlg->keySequence().toString(), SnippetsModel::KeySequenceRole);
        mModel->setData(index, dlg->keyword(), SnippetsModel::KeywordRole);
        mModel->setData(index, dlg->subject(), SnippetsModel::SubjectRole);
        mModel->setData(index, dlg->to(), SnippetsModel::ToRole);
        mModel->setData(index, dlg->cc(), SnippetsModel::CcRole);
        mModel->setData(index, dlg->bcc(), SnippetsModel::BccRole);
        mModel->setData(index, dlg->attachment(), SnippetsModel::AttachmentRole);

        Q_EMIT mModel->updateActionCollection(QString(),
                                              dlg->name(),
                                              dlg->keySequence(),
                                              dlg->text(),
                                              dlg->subject(),
                                              dlg->to(),
                                              dlg->cc(),
                                              dlg->bcc(),
                                              dlg->attachment());
        mDirty = true;
        save();
    }
    delete dlg;
}

void SnippetsManager::Private::slotAddNewDndSnippset(const QString &text)
{
    createSnippet(text);
}

void SnippetsManager::Private::dndDone()
{
    mDirty = true;
}

void SnippetsManager::Private::editSnippet()
{
    QModelIndex index = mSelectionModel->selectedIndexes().first();
    if (!index.isValid() || index.data(SnippetsModel::IsGroupRole).toBool()) {
        return;
    }

    const QModelIndex oldGroupIndex = currentGroupIndex();

    const QString oldSnippetName = index.data(SnippetsModel::NameRole).toString();

    QPointer<SnippetDialog> dlg = new SnippetDialog(mActionCollection, false, mParent);
    dlg->setWindowTitle(i18nc("@title:window", "Edit Snippet"));
    dlg->setGroupModel(mModel);
    dlg->setGroupIndex(oldGroupIndex);
    dlg->setName(oldSnippetName);
    dlg->setText(index.data(SnippetsModel::TextRole).toString());
    dlg->setKeyword(index.data(SnippetsModel::KeywordRole).toString());
    dlg->setSubject(index.data(SnippetsModel::SubjectRole).toString());
    dlg->setTo(index.data(SnippetsModel::ToRole).toString());
    dlg->setCc(index.data(SnippetsModel::CcRole).toString());
    dlg->setBcc(index.data(SnippetsModel::BccRole).toString());
    dlg->setAttachment(index.data(SnippetsModel::AttachmentRole).toString());
    dlg->setKeySequence(QKeySequence::fromString(index.data(SnippetsModel::KeySequenceRole).toString()));

    if (dlg->exec()) {
        const QModelIndex newGroupIndex = dlg->groupIndex();

        if (oldGroupIndex != newGroupIndex) {
            mModel->removeRow(index.row(), oldGroupIndex);
            mModel->insertRow(mModel->rowCount(newGroupIndex), newGroupIndex);

            index = mModel->index(mModel->rowCount(newGroupIndex) - 1, 0, newGroupIndex);
        }

        mModel->setData(index, dlg->name(), SnippetsModel::NameRole);
        mModel->setData(index, dlg->text(), SnippetsModel::TextRole);
        mModel->setData(index, dlg->keySequence().toString(), SnippetsModel::KeySequenceRole);
        mModel->setData(index, dlg->keyword(), SnippetsModel::KeywordRole);
        mModel->setData(index, dlg->subject(), SnippetsModel::SubjectRole);
        mModel->setData(index, dlg->to(), SnippetsModel::ToRole);
        mModel->setData(index, dlg->cc(), SnippetsModel::CcRole);
        mModel->setData(index, dlg->bcc(), SnippetsModel::BccRole);
        mModel->setData(index, dlg->attachment(), SnippetsModel::AttachmentRole);

        Q_EMIT mModel->updateActionCollection(oldSnippetName,
                                              dlg->name(),
                                              dlg->keySequence(),
                                              dlg->text(),
                                              dlg->subject(),
                                              dlg->to(),
                                              dlg->cc(),
                                              dlg->bcc(),
                                              dlg->attachment());
        mDirty = true;
        save();
    }
    delete dlg;
}

void SnippetsManager::Private::deleteSnippet()
{
    const QModelIndex index = mSelectionModel->selectedIndexes().first();

    const QString snippetName = index.data(SnippetsModel::NameRole).toString();

    if (KMessageBox::warningContinueCancel(nullptr,
                                           xi18nc("@info",
                                                  "Do you really want to remove snippet \"%1\"?<nl/>"
                                                  "<warning>There is no way to undo the removal.</warning>",
                                                  snippetName),
                                           QString(),
                                           KStandardGuiItem::remove())
        == KMessageBox::Cancel) {
        return;
    }

    mModel->removeRow(index.row(), currentGroupIndex());

    Q_EMIT mModel->updateActionCollection(snippetName, QString(), QKeySequence(), QString(), QString(), QString(), QString(), QString(), QString());
    mDirty = true;
    save();
}

void SnippetsManager::Private::addSnippetGroup()
{
    QPointer<SnippetDialog> dlg = new SnippetDialog(mActionCollection, true, mParent);
    dlg->setWindowTitle(i18nc("@title:window", "Add Group"));

    if (dlg->exec()) {
        if (!mModel->insertRow(mModel->rowCount(), QModelIndex())) {
            qCDebug(MAILCOMMON_LOG) << "unable to insert row";
            delete dlg;
            return;
        }

        const QModelIndex groupIndex = mModel->index(mModel->rowCount() - 1, 0, QModelIndex());
        mModel->setData(groupIndex, dlg->name(), SnippetsModel::NameRole);
        mDirty = true;
        save();
    }
    delete dlg;
}

void SnippetsManager::Private::editSnippetGroup()
{
    const QModelIndex groupIndex = currentGroupIndex();
    if (!groupIndex.isValid() || !groupIndex.data(SnippetsModel::IsGroupRole).toBool()) {
        return;
    }

    QPointer<SnippetDialog> dlg = new SnippetDialog(mActionCollection, true, mParent);
    dlg->setWindowTitle(i18nc("@title:window", "Edit Group"));
    const QString oldGroupName = groupIndex.data(SnippetsModel::NameRole).toString();
    dlg->setName(oldGroupName);

    if (dlg->exec()) {
        if (oldGroupName == dlg->name()) {
            delete dlg;
            return;
        }

        mModel->setData(groupIndex, dlg->name(), SnippetsModel::NameRole);
        mDirty = true;
        save();
    }
    delete dlg;
}

void SnippetsManager::Private::deleteSnippetGroup()
{
    const QModelIndex groupIndex = currentGroupIndex();
    if (!groupIndex.isValid()) {
        return;
    }

    const QString groupName = groupIndex.data(SnippetsModel::NameRole).toString();

    if (mModel->rowCount(groupIndex) > 0) {
        if (KMessageBox::warningContinueCancel(nullptr,
                                               xi18nc("@info",
                                                      "Do you really want to remove group \"%1\" along with all its snippets?<nl/>"
                                                      "<warning>There is no way to undo the removal.</warning>",
                                                      groupName),
                                               QString(),
                                               KStandardGuiItem::remove())
            == KMessageBox::Cancel) {
            return;
        }
    } else {
        if (KMessageBox::warningContinueCancel(nullptr,
                                               i18nc("@info", "Do you really want to remove group \"%1\"?", groupName),
                                               QString(),
                                               KStandardGuiItem::remove())
            == KMessageBox::Cancel) {
            return;
        }
    }

    mModel->removeRow(groupIndex.row(), QModelIndex());
    mDirty = true;
    save();
}

void SnippetsManager::Private::insertSelectedSnippet()
{
    if (!mSelectionModel->hasSelection()) {
        return;
    }

    const QModelIndex index = mSelectionModel->selectedIndexes().first();
    if (index.data(SnippetsModel::IsGroupRole).toBool()) {
        return;
    }

    const QString text = replaceVariables(index.data(SnippetsModel::TextRole).toString());
    const QString subject = replaceVariables(index.data(SnippetsModel::SubjectRole).toString());
    const QString to = index.data(SnippetsModel::ToRole).toString();
    const QString cc = index.data(SnippetsModel::CcRole).toString();
    const QString bcc = index.data(SnippetsModel::BccRole).toString();
    const QString attachment = index.data(SnippetsModel::AttachmentRole).toString();
    Q_EMIT q->insertSnippetInfo({subject, text, to, cc, bcc, attachment});
}

void SnippetsManager::Private::insertActionSnippet()
{
    auto action = qobject_cast<QAction *>(q->sender());
    if (!action) {
        return;
    }

    const QString text = replaceVariables(action->property("snippetText").toString());
    const QString subject = replaceVariables(action->property("snippetSubject").toString());
    const QString to = action->property("snippetTo").toString();
    const QString cc = action->property("snippetCc").toString();
    const QString bcc = action->property("snippetBcc").toString();
    const QString attachment = action->property("snippetAttachment").toString();
    Q_EMIT q->insertSnippetInfo({subject, text, to, cc, bcc, attachment});
}

void SnippetsManager::Private::initializeActionCollection()
{
    if (mActionCollection) {
        const QVector<SnippetsInfo> infos = mModel->snippetsInfo();
        for (const SnippetsInfo &info : infos) {
            initializeAction(info.newName, info.keySequence, info.text, info.subject, info.to, info.cc, info.bcc, info.attachment);
        }
    }
}

void SnippetsManager::Private::initializeAction(const QString &newName,
                                                const QKeySequence &keySequence,
                                                const QString &text,
                                                const QString &subject,
                                                const QString &to,
                                                const QString &cc,
                                                const QString &bcc,
                                                const QString &attachment)
{
    const QString actionName = i18nc("@action", "Snippet %1", newName);
    const QString normalizedName = QString(actionName).replace(QLatin1Char(' '), QLatin1Char('_'));

    QAction *action = mActionCollection->addAction(normalizedName, q);
    connect(action, &QAction::triggered, q, [this]() {
        insertActionSnippet();
    });
    action->setProperty("snippetText", text);
    action->setProperty("snippetSubject", subject);
    action->setProperty("snippetTo", to);
    action->setProperty("snippetCc", cc);
    action->setProperty("snippetBcc", bcc);
    action->setProperty("snippetAttachment", attachment);
    action->setText(actionName);
    mActionCollection->setDefaultShortcut(action, keySequence);
}

void SnippetsManager::Private::updateActionCollection(const QString &oldName,
                                                      const QString &newName,
                                                      const QKeySequence &keySequence,
                                                      const QString &text,
                                                      const QString &subject,
                                                      const QString &to,
                                                      const QString &cc,
                                                      const QString &bcc,
                                                      const QString &attachment)
{
    // remove previous action in case that the name changed
    if (!oldName.isEmpty() && mActionCollection) {
        const QString actionName = i18nc("@action", "Snippet %1", oldName);
        const QString normalizedName = QString(actionName).replace(QLatin1Char(' '), QLatin1Char('_'));

        QAction *action = mActionCollection->action(normalizedName);
        if (action) {
            mActionCollection->removeAction(action);
        }
    }

    if (!newName.isEmpty()) {
        initializeAction(newName, keySequence, text, subject, to, cc, bcc, attachment);
    }
}

QString SnippetsManager::Private::replaceVariables(const QString &text)
{
    QString result = text;
    QString variableName;
    QString variableValue;
    QMap<QString, QString> localVariables(SnippetsModel::instance()->savedVariables());
    int iFound = -1;
    int iEnd = -1;
    QMap<QString, QString> tempLocalVariables(localVariables);
    do {
        // find the next variable by this regex
        iFound = text.indexOf(QRegularExpression(QStringLiteral("\\$[A-Za-z\\-_0-9\\s]*\\$")), iEnd + 1);
        if (iFound >= 0) {
            iEnd = text.indexOf(QLatin1Char('$'), iFound + 1) + 1;

            variableName = text.mid(iFound, iEnd - iFound);

            if (variableName != QLatin1String("$$")) { // if not double-delimiter
                if (!localVariables.contains(variableName)) { // and not already in map
                    QPointer<SnippetVariableDialog> dlg = new SnippetVariableDialog(variableName, &tempLocalVariables, mParent);
                    if (dlg->exec()) {
                        if (dlg->saveVariableIsChecked()) {
                            mDirty = true;
                        }
                        variableValue = dlg->variableValue();
                    } else {
                        delete dlg;
                        return QString();
                    }
                    delete dlg;
                } else {
                    variableValue = localVariables.value(variableName);
                }
            } else {
                variableValue = QLatin1Char('$'); // if double-delimiter -> replace by single character
            }

            result.replace(variableName, variableValue);
            localVariables[variableName] = variableValue;
        }
    } while (iFound != -1);
    SnippetsModel::instance()->setSavedVariables(tempLocalVariables);

    return result;
}

void SnippetsManager::Private::save()
{
    if (!mDirty) {
        return;
    }

    SnippetsModel::instance()->save();
    mDirty = false;
}

SnippetsManager::SnippetsManager(KActionCollection *actionCollection, QObject *parent, QWidget *parentWidget)
    : QObject(parent)
    , d(new Private(this, parentWidget))
{
    d->mModel = SnippetsModel::instance();
    connect(d->mModel,
            &SnippetsModel::updateActionCollection,
            this,
            [this](const QString &oldName,
                   const QString &newName,
                   const QKeySequence &keySequence,
                   const QString &text,
                   const QString &subject,
                   const QString &to,
                   const QString &cc,
                   const QString &bcc,
                   const QString &attachment) {
                d->updateActionCollection(oldName, newName, keySequence, text, subject, to, cc, bcc, attachment);
            });
    d->mSelectionModel = new QItemSelectionModel(d->mModel);
    d->mActionCollection = actionCollection;

    d->mAddSnippetAction = new QAction(i18n("Add Snippet..."), this);
    d->mAddSnippetAction->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    d->mEditSnippetAction = new QAction(i18n("Edit Snippet..."), this);
    d->mEditSnippetAction->setIcon(QIcon::fromTheme(QStringLiteral("document-properties")));
    d->mDeleteSnippetAction = new QAction(i18n("Remove Snippet"), this);
    d->mDeleteSnippetAction->setIcon(QIcon::fromTheme(QStringLiteral("edit-delete")));

    d->mAddSnippetGroupAction = new QAction(i18n("Add Group..."), this);
    d->mAddSnippetGroupAction->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    d->mEditSnippetGroupAction = new QAction(i18n("Rename Group..."), this);
    d->mEditSnippetGroupAction->setIcon(QIcon::fromTheme(QStringLiteral("edit-rename")));
    d->mDeleteSnippetGroupAction = new QAction(i18n("Remove Group"), this);
    d->mDeleteSnippetGroupAction->setIcon(QIcon::fromTheme(QStringLiteral("edit-delete")));

    d->mInsertSnippetAction = new QAction(i18n("Insert Snippet"), this);
    d->mInsertSnippetAction->setIcon(QIcon::fromTheme(QStringLiteral("insert-text")));

    connect(d->mSelectionModel, &QItemSelectionModel::selectionChanged, this, [this]() {
        d->selectionChanged();
    });
    connect(d->mModel, &SnippetsModel::dndDone, this, [this]() {
        d->dndDone();
    });
    connect(d->mModel, &SnippetsModel::addNewDndSnippset, this, [this](const QString &str) {
        d->slotAddNewDndSnippset(str);
    });

    connect(d->mAddSnippetAction, &QAction::triggered, this, [this]() {
        d->addSnippet();
    });
    connect(d->mEditSnippetAction, &QAction::triggered, this, [this]() {
        d->editSnippet();
    });
    connect(d->mDeleteSnippetAction, &QAction::triggered, this, [this]() {
        d->deleteSnippet();
    });

    connect(d->mAddSnippetGroupAction, &QAction::triggered, this, [this]() {
        d->addSnippetGroup();
    });
    connect(d->mEditSnippetGroupAction, &QAction::triggered, this, [this]() {
        d->editSnippetGroup();
    });
    connect(d->mDeleteSnippetGroupAction, &QAction::triggered, this, [this]() {
        d->deleteSnippetGroup();
    });

    connect(d->mInsertSnippetAction, &QAction::triggered, this, [this]() {
        d->insertSelectedSnippet();
    });

    d->initializeActionCollection();
    d->selectionChanged();
    connect(this, &SnippetsManager::insertSnippet, this, [this]() {
        d->insertSelectedSnippet();
    });
}

SnippetsManager::~SnippetsManager()
{
    d->save();
    delete d;
}

QAbstractItemModel *SnippetsManager::model() const
{
    return d->mModel;
}

QItemSelectionModel *SnippetsManager::selectionModel() const
{
    return d->mSelectionModel;
}

QAction *SnippetsManager::addSnippetAction() const
{
    return d->mAddSnippetAction;
}

QAction *SnippetsManager::editSnippetAction() const
{
    return d->mEditSnippetAction;
}

QAction *SnippetsManager::deleteSnippetAction() const
{
    return d->mDeleteSnippetAction;
}

QAction *SnippetsManager::addSnippetGroupAction() const
{
    return d->mAddSnippetGroupAction;
}

QAction *SnippetsManager::editSnippetGroupAction() const
{
    return d->mEditSnippetGroupAction;
}

QAction *SnippetsManager::deleteSnippetGroupAction() const
{
    return d->mDeleteSnippetGroupAction;
}

QAction *SnippetsManager::insertSnippetAction() const
{
    return d->mInsertSnippetAction;
}

bool SnippetsManager::snippetGroupSelected() const
{
    if (d->mSelectionModel->selectedIndexes().isEmpty()) {
        return false;
    }

    return d->mSelectionModel->selectedIndexes().first().data(SnippetsModel::IsGroupRole).toBool();
}

QString SnippetsManager::selectedName() const
{
    if (d->mSelectionModel->selectedIndexes().isEmpty()) {
        return QString();
    }

    return d->mSelectionModel->selectedIndexes().first().data(SnippetsModel::NameRole).toString();
}

#include "moc_snippetsmanager.cpp"

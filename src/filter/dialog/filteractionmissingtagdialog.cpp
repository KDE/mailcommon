/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionmissingtagdialog.h"
#include <KLocalizedString>
#include <KSharedConfig>

#include "tag/addtagdialog.h"

#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QLabel>
#include <QListWidget>
#include <QPointer>
#include <QPushButton>
#include <QVBoxLayout>

using namespace MailCommon;

FilterActionMissingTagDialog::FilterActionMissingTagDialog(const QMap<QUrl, QString> &tagList,
                                                           const QString &filtername,
                                                           const QString &argsStr,
                                                           QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(i18nc("@title:window", "Select Tag"));
    auto mainLayout = new QVBoxLayout(this);

    auto label = new QLabel(i18n("Tag was \"%1\".", argsStr));
    label->setObjectName(QStringLiteral("tagnamelabel"));
    mainLayout->addWidget(label);

    label = new QLabel(this);
    label->setObjectName(QStringLiteral("filtername"));
    label->setText(
        i18n("Filter tag is missing. "
             "Please select a tag to use with filter \"%1\"",
             filtername));
    label->setWordWrap(true);
    mainLayout->addWidget(label);
    mTagList = new QListWidget(this);
    mTagList->setObjectName(QStringLiteral("taglist"));

    QMap<QUrl, QString>::const_iterator map = tagList.constBegin();
    const QMap<QUrl, QString>::const_iterator mapEnd = tagList.constEnd();
    for (; map != mapEnd; ++map) {
        auto item = new QListWidgetItem(map.value());
        item->setData(UrlData, map.key().toString());
        mTagList->addItem(item);
    }

    connect(mTagList, &QListWidget::itemDoubleClicked, this, &FilterActionMissingTagDialog::accept);
    mainLayout->addWidget(mTagList);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    auto user1Button = new QPushButton(this);
    user1Button->setObjectName(QStringLiteral("addtag"));
    user1Button->setText(i18n("Add Tag..."));
    buttonBox->addButton(user1Button, QDialogButtonBox::ActionRole);

    connect(user1Button, &QPushButton::clicked, this, &FilterActionMissingTagDialog::slotAddTag);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &FilterActionMissingTagDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterActionMissingTagDialog::reject);
    mainLayout->addWidget(buttonBox);
    okButton->setDefault(true);
    readConfig();
}

FilterActionMissingTagDialog::~FilterActionMissingTagDialog()
{
    writeConfig();
}

void FilterActionMissingTagDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "FilterActionMissingTagDialog");

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void FilterActionMissingTagDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "FilterActionMissingTagDialog");
    group.writeEntry("Size", size());
}

QString FilterActionMissingTagDialog::selectedTag() const
{
    if (mTagList->currentItem()) {
        return mTagList->currentItem()->data(UrlData).toString();
    }
    return QString();
}

void FilterActionMissingTagDialog::slotAddTag()
{
    QPointer<MailCommon::AddTagDialog> dlg = new MailCommon::AddTagDialog(QList<KActionCollection *>(), this);
    if (dlg->exec()) {
        auto item = new QListWidgetItem(dlg->label());
        item->setData(UrlData, dlg->tag().url().url());
        mTagList->addItem(item);
    }
    delete dlg;
}

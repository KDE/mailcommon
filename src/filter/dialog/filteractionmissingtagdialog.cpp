/*
   Copyright (C) 2016 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/


#include "filteractionmissingtagdialog.h"
#include <KLocalizedString>
#include <KSharedConfig>

#include "tag/addtagdialog.h"

#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPointer>

using namespace MailCommon;

FilterActionMissingTagDialog::FilterActionMissingTagDialog(
    const QMap<QUrl, QString> &tagList, const QString &filtername,
    const QString &argsStr, QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(i18n("Select Tag"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QWidget *mainWidget = new QWidget(this);
    mainLayout->addWidget(mainWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    QPushButton *user1Button = new QPushButton;
    buttonBox->addButton(user1Button, QDialogButtonBox::ActionRole);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &FilterActionMissingTagDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterActionMissingTagDialog::reject);
    mainLayout->addWidget(buttonBox);
    okButton->setDefault(true);
    user1Button->setText(i18n("Add Tag..."));
    QVBoxLayout *lay = new QVBoxLayout(mainWidget);
    QLabel *label = new QLabel(i18n("Tag was \"%1\".", argsStr));
    lay->addWidget(label);

    label = new QLabel(this);
    label->setText(i18n("Filter tag is missing. "
                        "Please select a tag to use with filter \"%1\"",
                        filtername));
    label->setWordWrap(true);
    lay->addWidget(label);
    mTagList = new QListWidget(this);

    QMapIterator<QUrl, QString> map(tagList);
    while (map.hasNext()) {
        map.next();
        QListWidgetItem *item = new QListWidgetItem(map.value());
        item->setData(UrlData, map.key().toString());
        mTagList->addItem(item);
    }

    connect(user1Button, &QPushButton::clicked, this, &FilterActionMissingTagDialog::slotAddTag);
    connect(mTagList, &QListWidget::itemDoubleClicked, this, &FilterActionMissingTagDialog::accept);
    lay->addWidget(mTagList);
    readConfig();
}

FilterActionMissingTagDialog::~FilterActionMissingTagDialog()
{
    writeConfig();
}

void FilterActionMissingTagDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "FilterActionMissingTagDialog");

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void FilterActionMissingTagDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "FilterActionMissingTagDialog");
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
    if (dlg->exec())  {
        QListWidgetItem *item = new QListWidgetItem(dlg->label());
        item->setData(UrlData, dlg->tag().url().url());
        mTagList->addItem(item);
    }
    delete dlg;
}


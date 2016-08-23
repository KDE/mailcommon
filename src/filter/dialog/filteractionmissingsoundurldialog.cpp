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

#include "filteractionmissingsoundurldialog.h"
#include "kernel/mailkernel.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KUrlRequester>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace MailCommon;

FilterActionMissingSoundUrlDialog::FilterActionMissingSoundUrlDialog(const QString &filtername,
        const QString &argStr,
        QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QWidget *mainWidget = new QWidget(this);
    mainLayout->addWidget(mainWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    QPushButton *user1Button = new QPushButton;
    buttonBox->addButton(user1Button, QDialogButtonBox::ActionRole);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &FilterActionMissingSoundUrlDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &FilterActionMissingSoundUrlDialog::reject);
    mainLayout->addWidget(buttonBox);
    okButton->setDefault(true);
    setWindowTitle(i18n("Select sound"));
    QVBoxLayout *lay = new QVBoxLayout(mainWidget);
    QLabel *label = new QLabel(i18n("Sound file was \"%1\".", argStr));
    lay->addWidget(label);

    label = new QLabel(this);
    label->setText(i18n("Sound file is missing. "
                        "Please select a sound to use with filter \"%1\"",
                        filtername));
    label->setWordWrap(true);
    lay->addWidget(label);
    mUrlWidget = new KUrlRequester(this);
    lay->addWidget(mUrlWidget);
    readConfig();
}

FilterActionMissingSoundUrlDialog::~FilterActionMissingSoundUrlDialog()
{
    writeConfig();
}

QString FilterActionMissingSoundUrlDialog::soundUrl() const
{
    return mUrlWidget->url().path();
}

void FilterActionMissingSoundUrlDialog::readConfig()
{
    KConfigGroup group(KernelIf->config(), "FilterActionMissingSoundUrlDialog");

    const QSize size = group.readEntry("Size", QSize(500, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void FilterActionMissingSoundUrlDialog::writeConfig()
{
    KConfigGroup group(KernelIf->config(), "FilterActionMissingSoundUrlDialog");
    group.writeEntry("Size", size());
}


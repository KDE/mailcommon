/*
 * Copyright (c) 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "filteractionaddtag.h"
#include "filter/filtermanager.h"
#include "filter/dialog/filteractionmissingtagdialog.h"

#include <QPointer>
#include <QComboBox>

#include <AkonadiCore/Tag>

using namespace MailCommon;

FilterAction *FilterActionAddTag::newAction()
{
    return new FilterActionAddTag;
}

FilterActionAddTag::FilterActionAddTag(QObject *parent)
    : FilterAction(QStringLiteral("add tag"), i18n("Add Tag"), parent)
{
    mList = FilterManager::instance()->tagList();
    connect(FilterManager::instance(), &FilterManager::tagListingFinished, this, &FilterActionAddTag::slotTagListingFinished);
}

QWidget *FilterActionAddTag::createParamWidget(QWidget *parent) const
{
    mComboBox = new QComboBox(parent);
    mComboBox->setMinimumWidth(50);
    mComboBox->setEditable(false);
    QMapIterator<QUrl, QString> i(mList);
    while (i.hasNext()) {
        i.next();
        mComboBox->addItem(i.value(), i.key());
    }

    setParamWidgetValue(mComboBox);

    connect(mComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FilterActionAddTag::filterActionModified);

    return mComboBox;
}

void FilterActionAddTag::applyParamWidgetValue(QWidget *paramWidget)
{
    auto combo = static_cast<QComboBox*>(paramWidget);
    mParameter = combo->itemData(combo->currentIndex()).toString();
}

void FilterActionAddTag::setParamWidgetValue(QWidget *paramWidget) const
{
    const int index = static_cast<QComboBox*>(paramWidget)->findData(mParameter);

    static_cast<QComboBox*>(paramWidget)->setCurrentIndex(index < 0 ? 0 : index);
}

void FilterActionAddTag::clearParamWidget(QWidget *paramWidget) const
{
    static_cast<QComboBox*>(paramWidget)->setCurrentIndex(0);
}

bool FilterActionAddTag::isEmpty() const
{
    return mParameter.isEmpty();
}

void FilterActionAddTag::slotTagListingFinished()
{
    if (mComboBox) {
        mList = FilterManager::instance()->tagList();
        mComboBox->clear();
        fillComboBox();
    }
}

void FilterActionAddTag::fillComboBox()
{
    QMapIterator<QUrl, QString> i(mList);
    while (i.hasNext()) {
        i.next();
        mComboBox->addItem(i.value(), i.key());
    }
}

bool FilterActionAddTag::argsFromStringInteractive(const QString &argsStr, const QString &filterName)
{
    bool needUpdate = false;
    argsFromString(argsStr);
    if (mList.isEmpty()) {
        return needUpdate;
    }
    const bool index = mList.contains(QUrl(mParameter));
    if (!index) {
        QPointer<MailCommon::FilterActionMissingTagDialog> dlg = new MailCommon::FilterActionMissingTagDialog(mList, filterName, argsStr);
        if (dlg->exec()) {
            mParameter = dlg->selectedTag();
            needUpdate = true;
        }
        delete dlg;
    }
    return needUpdate;
}

FilterAction::ReturnCode FilterActionAddTag::process(ItemContext &context, bool) const
{
    if (!mList.contains(QUrl(mParameter))) {
        return ErrorButGoOn;
    }
    context.item().setTag(Akonadi::Tag::fromUrl(QUrl(mParameter)));
    context.setNeedsFlagStore();

    return GoOn;
}

SearchRule::RequiredPart FilterActionAddTag::requiredPart() const
{
    return SearchRule::Envelope;
}

void FilterActionAddTag::argsFromString(const QString &argsStr)
{
    if (mList.isEmpty()) {
        mParameter = argsStr;
        return;
    }
    if (mList.contains(QUrl(argsStr))) {
        mParameter = argsStr;
        return;
    }
    if (!mList.isEmpty()) {
        mParameter = mList.cbegin().value();
    }
}

QString FilterActionAddTag::argsAsString() const
{
    return mParameter;
}

QString FilterActionAddTag::displayString() const
{
    return label() + QLatin1String(" \"") + argsAsString().toHtmlEscaped() + QLatin1String("\"");
}

QString FilterActionAddTag::informationAboutNotValidAction() const
{
    const QString info = name() + QLatin1Char('\n') + i18n("No tag selected.");
    return info;
}

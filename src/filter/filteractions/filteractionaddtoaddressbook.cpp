/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractionaddtoaddressbook.h"

#include <Akonadi/Contact/AddContactJob>

#include <AkonadiWidgets/TagSelectionDialog>
#include <AkonadiWidgets/TagWidget>

#include <CollectionComboBox>
#include <KComboBox>
#include <KContacts/Addressee>
#include <KEmailAddress>
#include <KLocalizedString>

#include <QGridLayout>
#include <QLabel>

using namespace MailCommon;

FilterAction *FilterActionAddToAddressBook::newAction()
{
    return new FilterActionAddToAddressBook;
}

FilterActionAddToAddressBook::FilterActionAddToAddressBook(QObject *parent)
    : FilterActionWithStringList(QStringLiteral("add to address book"), i18n("Add to Address Book"), parent)
    , mFromStr(i18nc("Email sender", "From"))
    , mToStr(i18nc("Email recipient", "To"))
    , mCCStr(i18n("CC"))
    , mBCCStr(i18n("BCC"))
    , mHeaderType(UnknownHeader)
    , mCollectionId(-1)
    , mCategory(i18n("KMail Filter"))
{
}

bool FilterActionAddToAddressBook::isEmpty() const
{
    return (mCollectionId == -1) || (mHeaderType == UnknownHeader);
}

FilterAction::ReturnCode FilterActionAddToAddressBook::process(ItemContext &context, bool) const
{
    if (isEmpty()) {
        return ErrorButGoOn;
    }

    const auto msg = context.item().payload<KMime::Message::Ptr>();

    QString headerLine;
    switch (mHeaderType) {
    case FromHeader:
        headerLine = msg->from()->asUnicodeString();
        break;
    case ToHeader:
        headerLine = msg->to()->asUnicodeString();
        break;
    case CcHeader:
        headerLine = msg->cc()->asUnicodeString();
        break;
    case BccHeader:
        headerLine = msg->bcc()->asUnicodeString();
        break;
    case UnknownHeader:
        break;
    }
    if (headerLine.isEmpty()) {
        return ErrorButGoOn;
    }

    const QStringList emails = KEmailAddress::splitAddressList(headerLine);

    for (const QString &singleEmail : emails) {
        QString name, email;
        KContacts::Addressee::parseEmailAddress(singleEmail, name, email);

        KContacts::Addressee contact;
        contact.setNameFromString(name);
        contact.insertEmail(email, true);
        if (!mCategory.isEmpty()) {
            contact.setCategories(mCategory.split(QLatin1Char(';')));
        }

        auto job = new Akonadi::AddContactJob(contact, Akonadi::Collection(mCollectionId));
        job->showMessageBox(false);
        job->start();
    }

    return GoOn;
}

SearchRule::RequiredPart FilterActionAddToAddressBook::requiredPart() const
{
    return SearchRule::Envelope;
}

QWidget *FilterActionAddToAddressBook::createParamWidget(QWidget *parent) const
{
    auto widget = new QWidget(parent);
    auto layout = new QGridLayout(widget);

    const auto headerCombo = new KComboBox(widget);
    headerCombo->setMinimumWidth(50);
    headerCombo->setObjectName(QStringLiteral("HeaderComboBox"));
    layout->addWidget(headerCombo, 0, 0, 2, 1, Qt::AlignVCenter);

    auto label = new QLabel(i18n("with category"), widget);
    label->setObjectName(QStringLiteral("label_with_category"));
    layout->addWidget(label, 0, 1);

    auto categoryEdit = new Akonadi::TagWidget(widget);
    categoryEdit->setObjectName(QStringLiteral("CategoryEdit"));
    layout->addWidget(categoryEdit, 0, 2);

    label = new QLabel(i18n("in address book"), widget);
    label->setObjectName(QStringLiteral("label_in_addressbook"));
    layout->addWidget(label, 1, 1);

    auto collectionComboBox = new Akonadi::CollectionComboBox(widget);
    collectionComboBox->setMimeTypeFilter(QStringList() << KContacts::Addressee::mimeType());
    collectionComboBox->setAccessRightsFilter(Akonadi::Collection::CanCreateItem);

    collectionComboBox->setObjectName(QStringLiteral("AddressBookComboBox"));
    collectionComboBox->setToolTip(
        i18n("This defines the preferred address book.\n"
             "If it is not accessible, the filter will fallback to the default address book."));
    layout->addWidget(collectionComboBox, 1, 2);
    connect(headerCombo, QOverload<int>::of(&KComboBox::currentIndexChanged), this, &FilterActionAddToAddressBook::filterActionModified);
    connect(collectionComboBox, QOverload<int>::of(&Akonadi::CollectionComboBox::activated), this, &FilterActionAddToAddressBook::filterActionModified);
    connect(categoryEdit, &Akonadi::TagWidget::selectionChanged, this, &FilterActionAddToAddressBook::filterActionModified);

    setParamWidgetValue(widget);

    return widget;
}

namespace
{
Akonadi::Tag::List namesToTags(const QStringList &names)
{
    Akonadi::Tag::List tags;
    tags.reserve(names.size());
    std::transform(names.cbegin(), names.cend(), std::back_inserter(tags), [](const QString &name) {
        return Akonadi::Tag{name};
    });
    return tags;
}

QStringList tagsToNames(const Akonadi::Tag::List &tags)
{
    QStringList names;
    names.reserve(tags.size());
    std::transform(tags.cbegin(), tags.cend(), std::back_inserter(names), std::bind(&Akonadi::Tag::name, std::placeholders::_1));
    return names;
}
}

void FilterActionAddToAddressBook::setParamWidgetValue(QWidget *paramWidget) const
{
    const auto headerCombo = paramWidget->findChild<KComboBox *>(QStringLiteral("HeaderComboBox"));
    Q_ASSERT(headerCombo);
    headerCombo->clear();
    headerCombo->addItem(mFromStr, FromHeader);
    headerCombo->addItem(mToStr, ToHeader);
    headerCombo->addItem(mCCStr, CcHeader);
    headerCombo->addItem(mBCCStr, BccHeader);

    headerCombo->setCurrentIndex(headerCombo->findData(mHeaderType));

    auto categoryEdit = paramWidget->findChild<Akonadi::TagWidget *>(QStringLiteral("CategoryEdit"));
    Q_ASSERT(categoryEdit);
    categoryEdit->setSelection(namesToTags(mCategory.split(QLatin1Char(';'))));

    auto *collectionComboBox = paramWidget->findChild<Akonadi::CollectionComboBox *>(QStringLiteral("AddressBookComboBox"));
    Q_ASSERT(collectionComboBox);
    collectionComboBox->setDefaultCollection(Akonadi::Collection(mCollectionId));
    collectionComboBox->setProperty("collectionId", mCollectionId);
}

void FilterActionAddToAddressBook::applyParamWidgetValue(QWidget *paramWidget)
{
    const auto headerCombo = paramWidget->findChild<QComboBox *>(QStringLiteral("HeaderComboBox"));
    Q_ASSERT(headerCombo);
    mHeaderType = static_cast<HeaderType>(headerCombo->itemData(headerCombo->currentIndex()).toInt());

    const auto categoryEdit = paramWidget->findChild<Akonadi::TagWidget *>(QStringLiteral("CategoryEdit"));
    Q_ASSERT(categoryEdit);
    mCategory = tagsToNames(categoryEdit->selection()).join(QLatin1Char(';'));

    const Akonadi::CollectionComboBox *collectionComboBox = paramWidget->findChild<Akonadi::CollectionComboBox *>(QStringLiteral("AddressBookComboBox"));
    Q_ASSERT(collectionComboBox);
    const Akonadi::Collection collection = collectionComboBox->currentCollection();

    // it might be that the model of collectionComboBox has not finished loading yet, so
    // we use the previously 'stored' value from the 'collectionId' property
    if (collection.isValid()) {
        mCollectionId = collection.id();
        connect(collectionComboBox,
                QOverload<int>::of(&Akonadi::CollectionComboBox::currentIndexChanged),
                this,
                &FilterActionAddToAddressBook::filterActionModified);
    } else {
        const QVariant value = collectionComboBox->property("collectionId");
        if (value.isValid()) {
            mCollectionId = value.toLongLong();
        }
    }
}

void FilterActionAddToAddressBook::clearParamWidget(QWidget *paramWidget) const
{
    const auto headerCombo = paramWidget->findChild<QComboBox *>(QStringLiteral("HeaderComboBox"));
    Q_ASSERT(headerCombo);
    headerCombo->setCurrentIndex(0);

    auto categoryEdit = paramWidget->findChild<Akonadi::TagWidget *>(QStringLiteral("CategoryEdit"));
    Q_ASSERT(categoryEdit);
    categoryEdit->setSelection(namesToTags(mCategory.split(QLatin1Char(';'))));
}

QString FilterActionAddToAddressBook::argsAsString() const
{
    QString result;

    switch (mHeaderType) {
    case FromHeader:
        result = QStringLiteral("From");
        break;
    case ToHeader:
        result = QStringLiteral("To");
        break;
    case CcHeader:
        result = QStringLiteral("CC");
        break;
    case BccHeader:
        result = QStringLiteral("BCC");
        break;
    case UnknownHeader:
        break;
    }

    result += QLatin1Char('\t');
    result += QString::number(mCollectionId);
    result += QLatin1Char('\t');
    result += mCategory;

    return result;
}

void FilterActionAddToAddressBook::argsFromString(const QString &argsStr)
{
    const QStringList parts = argsStr.split(QLatin1Char('\t'), Qt::KeepEmptyParts);
    const QString firstElement = parts[0];
    if (firstElement == QLatin1String("From")) {
        mHeaderType = FromHeader;
    } else if (firstElement == QLatin1String("To")) {
        mHeaderType = ToHeader;
    } else if (firstElement == QLatin1String("CC")) {
        mHeaderType = CcHeader;
    } else if (firstElement == QLatin1String("BCC")) {
        mHeaderType = BccHeader;
    } else {
        mHeaderType = UnknownHeader;
    }
    if (parts.count() >= 2) {
        mCollectionId = parts[1].toLongLong();
    }

    if (parts.count() < 3) {
        mCategory.clear();
    } else {
        mCategory = parts[2];
    }
}

QString FilterActionAddToAddressBook::informationAboutNotValidAction() const
{
    QString result;
    if (mHeaderType == UnknownHeader) {
        result = i18n("Header type selected is unknown.");
    }
    if (mCollectionId == -1) {
        if (!result.isEmpty()) {
            result += QLatin1Char('\n');
        }
        result += i18n("No addressbook selected.");
    }
    return result;
}

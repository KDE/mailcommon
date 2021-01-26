/*
 * SPDX-FileCopyrightText: 1996-1998 Stefan Taferner <taferner@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include "filteractiondict.h"

#include "filteractionaddheader.h"
#include "filteractionaddtag.h"
#include "filteractionaddtoaddressbook.h"
#include "filteractioncopy.h"
#include "filteractiondecrypt.h"
#include "filteractiondelete.h"
#include "filteractionencrypt.h"
#include "filteractionexec.h"
#include "filteractionforward.h"
#include "filteractionmove.h"
#include "filteractionpipethrough.h"
#include "filteractionplaysound.h"
#include "filteractionredirect.h"
#include "filteractionremoveheader.h"
#include "filteractionreplyto.h"
#include "filteractionrewriteheader.h"
#include "filteractionsendfakedisposition.h"
#include "filteractionsendreceipt.h"
#include "filteractionsetidentity.h"
#include "filteractionsetstatus.h"
#include "filteractionsettransport.h"
#include "filteractionunsetstatus.h"

using namespace MailCommon;

//=============================================================================
//
//   Filter  Action  Dictionary
//
//=============================================================================
FilterActionDict::~FilterActionDict()
{
    qDeleteAll(mList);
}

void FilterActionDict::init()
{
    insert(FilterActionMove::newAction);
    insert(FilterActionCopy::newAction);
    insert(FilterActionSetIdentity::newAction);
    insert(FilterActionSetStatus::newAction);
    insert(FilterActionAddTag::newAction);
    insert(FilterActionSendFakeDisposition::newAction);
    insert(FilterActionSetTransport::newAction);
    insert(FilterActionReplyTo::newAction);
    insert(FilterActionForward::newAction);
    insert(FilterActionRedirect::newAction);
    insert(FilterActionSendReceipt::newAction);
    insert(FilterActionExec::newAction);
    insert(FilterActionPipeThrough::newAction);
    insert(FilterActionRemoveHeader::newAction);
    insert(FilterActionAddHeader::newAction);
    insert(FilterActionRewriteHeader::newAction);
    insert(FilterActionPlaySound::newAction);
    insert(FilterActionAddToAddressBook::newAction);
    insert(FilterActionDelete::newAction);
    insert(FilterActionUnsetStatus::newAction);
    insert(FilterActionEncrypt::newAction);
    insert(FilterActionDecrypt::newAction);
    // Register custom filter actions below this line.
}

// The int in the QDict constructor (41) must be a prime
// and should be greater than the double number of FilterAction types
FilterActionDict::FilterActionDict()
    : QMultiHash<QString, FilterActionDesc *>()
{
    init();
}

void FilterActionDict::insert(FilterActionNewFunc aNewFunc)
{
    FilterAction *action = aNewFunc();
    auto desc = new FilterActionDesc;
    desc->name = action->name();
    desc->label = action->label();
    desc->create = aNewFunc;

    QMultiHash<QString, FilterActionDesc *>::insert(desc->name, desc);
    QMultiHash<QString, FilterActionDesc *>::insert(desc->label, desc);
    mList.append(desc);

    delete action;
}

const QVector<FilterActionDesc *> &FilterActionDict::list() const
{
    return mList;
}

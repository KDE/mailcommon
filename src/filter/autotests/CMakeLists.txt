# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
set(mailcommon_filter_test_LIBS
    Qt::Test
    KF6::KIOWidgets
    KPim6::MailTransport
    KF6::SyntaxHighlighting
    KPim6::MailCommon
)

add_definitions(-DTEST_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\")

macro(add_mailcommon_filter_test _name)
    ecm_add_test(${ARGN}
        TEST_NAME ${_name}
        NAME_PREFIX "mailcommon-filter-"
        LINK_LIBRARIES ${mailcommon_filter_test_LIBS}
    )
endmacro()

macro(add_mailcommon_filter_akonadi_test _name _sources)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
    add_akonadi_isolated_test_advanced(${_name} "${ARGN}" "${mailcommon_filter_test_LIBS}")
endmacro()

add_mailcommon_filter_test(itemcontexttest
    itemcontexttest.cpp itemcontexttest.h
)

add_mailcommon_filter_test(filteractionpipethroughtest
   filteractionpipethroughtest.cpp
   filteractionpipethroughtest.h
)

add_mailcommon_filter_test(filteractionheadertest
    filteractionaddheadertest.cpp
    filteractionaddheadertest.h
)

add_mailcommon_filter_test(filteractiondeletetest
    filteractiondeletetest.cpp
    filteractiondeletetest.h
)

add_mailcommon_filter_test(filteractionencrypttest
    filteractionencrypttest.cpp
    filteractionencrypttest.h
    gpghelper.cpp gpghelper.h
)

add_mailcommon_filter_test(filteractiondecrypttest
    filteractiondecrypttest.cpp
    filteractiondecrypttest.h
    gpghelper.cpp gpghelper.h
)

add_mailcommon_filter_test(filteractionrewriteheadertest
    filteractionrewriteheadertest.cpp
    filteractionrewriteheadertest.h
)

add_mailcommon_filter_test(filteractionsetstatustest
    filteractionsetstatustest.cpp
    filteractionsetstatustest.h
)

add_mailcommon_filter_test(filteractionreplytotest
    filteractionreplytotest.cpp
    filteractionreplytotest.h
)

add_mailcommon_filter_test(filteractionwithurltest
    filteractionwithurltest.cpp
    filteractionwithurltest.h
)

add_mailcommon_filter_test(filteractionremoveheadertest
    filteractionremoveheadertest.cpp
    filteractionremoveheadertest.h
)

add_mailcommon_filter_test(filteractionwithaddresstest
    filteractionwithaddresstest.cpp
    filteractionwithaddresstest.h
)

add_mailcommon_filter_akonadi_test(filteractionaddtoaddressbooktest.cpp
    filteractionaddtoaddressbooktest.cpp
    filteractionaddtoaddressbooktest.h
)

add_mailcommon_filter_test(filteractionplaysoundtest
    filteractionplaysoundtest.cpp
    filteractionplaysoundtest.h
)

add_mailcommon_filter_test(invalidfilterdialogtest
    invalidfilterdialogtest.cpp
    invalidfilterdialogtest.h
)

add_mailcommon_filter_test(invalidfilterwidgettest
    invalidfilterwidgettest.cpp
    invalidfilterwidgettest.h
)

add_mailcommon_filter_test(invalidfilterlistwidgettest
    invalidfilterlistwidgettest.cpp
    invalidfilterlistwidgettest.h
)

add_mailcommon_filter_test(filteractionunsetstatustest
    filteractionunsetstatustest.cpp
    filteractionunsetstatustest.h
)

add_mailcommon_filter_test(filteractionredirecttest
    filteractionredirecttest.cpp
    filteractionredirecttest.h
)

add_mailcommon_filter_test(invalidfilterinfotest
    invalidfilterinfotest.cpp
    invalidfilterinfotest.h
)

add_mailcommon_filter_test(filteractionsendfakedispositiontest
    filteractionsendfakedispositiontest.cpp
    filteractionsendfakedispositiontest.h
)

add_mailcommon_filter_test(filteractionforwardtest
    filteractionforwardtest.cpp
    filteractionforwardtest.h
)

add_mailcommon_filter_test(invalidfilterinfowidgettest
    invalidfilterinfowidgettest.cpp
    invalidfilterinfowidgettest.h
)

add_mailcommon_filter_test(filteractioncopytest
    filteractioncopytest.cpp
    filteractioncopytest.h
)

add_mailcommon_filter_test(filteractionmovetest
    filteractionmovetest.cpp
    filteractionmovetest.h
)

add_mailcommon_filter_test(filteractionexectest
    filteractionexectest.cpp
    filteractionexectest.h
)

add_mailcommon_filter_test(filteractionsendreceipttest
    filteractionsendreceipttest.cpp
    filteractionsendreceipttest.h
)

add_mailcommon_filter_test(filteractionsettransporttest
    filteractionsettransporttest.cpp
    filteractionsettransporttest.h
)

add_mailcommon_filter_test(filterconverttosieveresultdialog
    filterconverttosieveresultdialogtest.cpp
    filterconverttosieveresultdialogtest.h
)

add_mailcommon_filter_test(mailfiltertest
    mailfiltertest.cpp
    mailfiltertest.h
)

add_mailcommon_filter_test(filteractionmissingsoundurldialogtest
    filteractionmissingsoundurldialogtest.cpp
    filteractionmissingsoundurldialogtest.h
)

add_mailcommon_filter_test(filteractionmissingtemplatedialogtest
    filteractionmissingtemplatedialogtest.cpp
    filteractionmissingtemplatedialogtest.h
)

add_mailcommon_filter_test(filteractionmissingtransportdialogtest
    filteractionmissingtransportdialogtest.cpp
    filteractionmissingtransportdialogtest.h
)

add_mailcommon_filter_test(filteractionmissingaccountdialogtest
    filteractionmissingaccountdialogtest.cpp
    filteractionmissingaccountdialogtest.h
)

add_mailcommon_filter_test(filteractionmissingtagdialogtest
    filteractionmissingtagdialogtest.cpp
    filteractionmissingtagdialogtest.h
)

add_mailcommon_filter_akonadi_test(
    filteractionmissingidentitydialogtest.cpp
    filteractionmissingidentitydialogtest.h
    ../../../autotests/dummykernel.cpp ../../../autotests/dummykernel.h
)

add_mailcommon_filter_test(filterimporterpathcachetest
    filterimporterpathcachetest.cpp
    filterimporterpathcachetest.h
)

add_mailcommon_filter_test(filterlogtest
    filterlogtest.cpp
    filterlogtest.h
)

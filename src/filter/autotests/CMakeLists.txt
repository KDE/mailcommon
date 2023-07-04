# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
set(mailcommon_filter_test_LIBS Qt::Test Qt::Gui KPim6::AkonadiCore KPim6::AkonadiMime
                                KPim6::MailCommon KPim6::PimCommon Phonon::phonon4qt6
                                KF6::KIOWidgets KF6::IconThemes KPim6::TemplateParser
				KPim6::AkonadiContactWidgets
                                KPim6::MailTransport KF6::I18n KF6::SyntaxHighlighting KF6::Codecs
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

set(filter_common_SRCS)
ecm_qt_declare_logging_category(filter_common_SRCS HEADER mailcommon_debug.h IDENTIFIER MAILCOMMON_LOG CATEGORY_NAME org.kde.pim.mailcommon)

add_mailcommon_filter_test(itemcontexttest
    itemcontexttest.cpp itemcontexttest.h
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionpipethroughtest
   filteractionpipethroughtest.cpp
   filteractionpipethroughtest.h
   ../filteractions/filteractionpipethrough.cpp
   ../filteractions/filteractionwithcommand.cpp
   ../filteractions/filteractionwithurl.cpp
   ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionheadertest
    filteractionaddheadertest.cpp
    filteractionaddheadertest.h
    ../filteractions/filteractionaddheader.cpp
    ../filteractions/filteractionwithstringlist.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractiondeletetest
    filteractiondeletetest.cpp
    filteractiondeletetest.h
    ../filteractions/filteractiondelete.cpp
    ../filteractions/filteractionwithnone.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionencrypttest
    filteractionencrypttest.cpp
    filteractionencrypttest.h
    gpghelper.cpp gpghelper.h
    ../filteractions/filteractionencrypt.cpp
    ../filteractions/filteractionwithcrypto.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractiondecrypttest
    filteractiondecrypttest.cpp
    filteractiondecrypttest.h
    gpghelper.cpp gpghelper.h
    ../filteractions/filteractiondecrypt.cpp
    ../filteractions/filteractionwithcrypto.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionrewriteheadertest
    filteractionrewriteheadertest.cpp
    filteractionrewriteheadertest.h
    ../filteractions/filteractionrewriteheader.cpp
    ../filteractions/filteractionwithstringlist.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionsetstatustest
    filteractionsetstatustest.cpp
    filteractionsetstatustest.h
    ../filteractions/filteractionsetstatus.cpp
    ../filteractions/filteractionstatus.cpp
    ../filteractions/filteractionwithstringlist.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionreplytotest
    filteractionreplytotest.cpp
    filteractionreplytotest.h
    ../filteractions/filteractionreplyto.cpp
    ../filteractions/filteractionwithaddress.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionwithurltest
    filteractionwithurltest.cpp
    filteractionwithurltest.h
    ../filteractions/filteractionwithurl.cpp
    ../filteractions/filteractionwithaddress.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionremoveheadertest
    filteractionremoveheadertest.cpp
    filteractionremoveheadertest.h
    ../filteractions/filteractionremoveheader.cpp
    ../filteractions/filteractionwithstringlist.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionwithaddresstest
    filteractionwithaddresstest.cpp
    filteractionwithaddresstest.h
    ../filteractions/filteractionwithaddress.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_akonadi_test(filteractionaddtoaddressbooktest.cpp
    filteractionaddtoaddressbooktest.cpp
    filteractionaddtoaddressbooktest.h
    ../filteractions/filteractionaddtoaddressbook.cpp
    ../filteractions/filteractionwithstringlist.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionplaysoundtest
    filteractionplaysoundtest.cpp
    filteractionplaysoundtest.h
    ../filteractions/filteractionplaysound.cpp
    ../filteractions/filteractionwithtest.cpp
    ../filteractions/filteractionplaysound.cpp
    ../soundtestwidget.cpp
    ../dialog/filteractionmissingsoundurldialog.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(invalidfilterdialogtest
    invalidfilterdialogtest.cpp
    invalidfilterdialogtest.h
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(invalidfilterwidgettest
    invalidfilterwidgettest.cpp
    invalidfilterwidgettest.h
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(invalidfilterlistwidgettest
    invalidfilterlistwidgettest.cpp
    invalidfilterlistwidgettest.h
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionunsetstatustest
    filteractionunsetstatustest.cpp
    filteractionunsetstatustest.h
    ../filteractions/filteractionunsetstatus.cpp
    ../filteractions/filteractionstatus.cpp
    ../filteractions/filteractionwithstringlist.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionredirecttest
    filteractionredirecttest.cpp
    filteractionredirecttest.h
    ../filteractions/filteractionredirect.cpp
    ../filteractions/filteractionwithaddress.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(invalidfilterinfotest
    invalidfilterinfotest.cpp
    invalidfilterinfotest.h
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionsendfakedispositiontest
    filteractionsendfakedispositiontest.cpp
    filteractionsendfakedispositiontest.h
    ../filteractions/filteractionsendfakedisposition.cpp
    ../filteractions/filteractionwithstringlist.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionforwardtest
    filteractionforwardtest.cpp
    filteractionforwardtest.h
    ../filteractions/filteractionforward.cpp
    ../filteractions/filteractionwithaddress.cpp
    ../filteractions/filteractionwithstring.cpp
    ../dialog/filteractionmissingtemplatedialog.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(invalidfilterinfowidgettest
    invalidfilterinfowidgettest.cpp
    invalidfilterinfowidgettest.h
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractioncopytest
    filteractioncopytest.cpp
    filteractioncopytest.h
    ../filteractions/filteractioncopy.cpp
    ../filteractions/filteractionwithfolder.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionmovetest
    filteractionmovetest.cpp
    filteractionmovetest.h
    ../filteractions/filteractionmove.cpp
    ../filteractions/filteractionwithfolder.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionexectest
    filteractionexectest.cpp
    filteractionexectest.h
    ../filteractions/filteractionexec.cpp
    ../filteractions/filteractionwithcommand.cpp
    ../filteractions/filteractionwithurl.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionsendreceipttest
    filteractionsendreceipttest.cpp
    filteractionsendreceipttest.h
    ../filteractions/filteractionsendreceipt.cpp
    ../filteractions/filteractionwithnone.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionsettransporttest
    filteractionsettransporttest.cpp
    filteractionsettransporttest.h
    ../filteractions/filteractionsettransport.cpp
    ../dialog/filteractionmissingtransportdialog.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filterconverttosieveresultdialog
    filterconverttosieveresultdialogtest.cpp
    filterconverttosieveresultdialogtest.h
    ../filterconverter/filterconverttosieveresultdialog.cpp
    ../filterconverter/filterconverttosievepurposemenuwidget.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(mailfiltertest
    mailfiltertest.cpp
    mailfiltertest.h
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionmissingsoundurldialogtest
    filteractionmissingsoundurldialogtest.cpp
    filteractionmissingsoundurldialogtest.h
    ../dialog/filteractionmissingsoundurldialog.cpp
    )

add_mailcommon_filter_test(filteractionmissingtemplatedialogtest
    filteractionmissingtemplatedialogtest.cpp
    filteractionmissingtemplatedialogtest.h
    ../dialog/filteractionmissingtemplatedialog.cpp
    )

add_mailcommon_filter_test(filteractionmissingtransportdialogtest
    filteractionmissingtransportdialogtest.cpp
    filteractionmissingtransportdialogtest.h
    ../dialog/filteractionmissingtransportdialog.cpp
    )

add_mailcommon_filter_test(filteractionmissingaccountdialogtest
    filteractionmissingaccountdialogtest.cpp
    filteractionmissingaccountdialogtest.h
    ../dialog/filteractionmissingaccountdialog.cpp
    )

add_mailcommon_filter_test(filteractionmissingtagdialogtest
    filteractionmissingtagdialogtest.cpp
    filteractionmissingtagdialogtest.h
    ../dialog/filteractionmissingtagdialog.cpp
    )

add_mailcommon_filter_test(filteractionmissingidentitydialogtest
    filteractionmissingidentitydialogtest.cpp
    filteractionmissingidentitydialogtest.h
    ../dialog/filteractionmissingidentitydialog.cpp
    ../../../autotests/dummykernel.cpp ../../../autotests/dummykernel.h
    )

add_mailcommon_filter_test(filterimporterpathcachetest
    filterimporterpathcachetest.cpp
    filterimporterpathcachetest.h
    )

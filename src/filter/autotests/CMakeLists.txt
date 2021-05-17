set(mailcommon_filter_test_LIBS Qt::Test Qt::Gui KF5::AkonadiCore KF5::AkonadiMime
                                KF5::MailCommon KF5::PimCommon Phonon::phonon4qt5
                                KF5::KIOWidgets KF5::IconThemes KF5::TemplateParser
                                KF5::MailTransport KF5::I18n KF5::SyntaxHighlighting
)

if (KF5_USE_PURPOSE)
    set(mailcommon_filter_test_LIBS ${mailcommon_filter_test_LIBS} KF5::Purpose KF5::PurposeWidgets)
endif()

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
    itemcontexttest.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionpipethroughtest
   filteractionpipethroughtest.cpp
   ../filteractions/filteractionpipethrough.cpp
   ../filteractions/filteractionwithcommand.cpp
   ../filteractions/filteractionwithurl.cpp
   ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionheadertest
    filteractionaddheadertest.cpp
    ../filteractions/filteractionaddheader.cpp
    ../filteractions/filteractionwithstringlist.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractiondeletetest
    filteractiondeletetest.cpp
    ../filteractions/filteractiondelete.cpp
    ../filteractions/filteractionwithnone.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionencrypttest
    filteractionencrypttest.cpp
    gpghelper.cpp
    ../filteractions/filteractionencrypt.cpp
    ../filteractions/filteractionwithcrypto.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractiondecrypttest
    filteractiondecrypttest.cpp
    gpghelper.cpp
    ../filteractions/filteractiondecrypt.cpp
    ../filteractions/filteractionwithcrypto.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionrewriteheadertest
    filteractionrewriteheadertest.cpp
    ../filteractions/filteractionrewriteheader.cpp
    ../filteractions/filteractionwithstringlist.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionsetstatustest
    filteractionsetstatustest.cpp
    ../filteractions/filteractionsetstatus.cpp
    ../filteractions/filteractionstatus.cpp
    ../filteractions/filteractionwithstringlist.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionreplytotest
    filteractionreplytotest.cpp
    ../filteractions/filteractionreplyto.cpp
    ../filteractions/filteractionwithaddress.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionwithurltest
    filteractionwithurltest.cpp
    ../filteractions/filteractionwithurl.cpp
    ../filteractions/filteractionwithaddress.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionremoveheadertest
    filteractionremoveheadertest.cpp
    ../filteractions/filteractionremoveheader.cpp
    ../filteractions/filteractionwithstringlist.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionwithaddresstest
    filteractionwithaddresstest.cpp
    ../filteractions/filteractionwithaddress.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

if (KDEPIM_RUN_AKONADI_TEST)
add_mailcommon_filter_akonadi_test(filteractionaddtoaddressbooktest.cpp
    filteractionaddtoaddressbooktest.cpp
    ../filteractions/filteractionaddtoaddressbook.cpp
    ../filteractions/filteractionwithstringlist.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)
endif()

add_mailcommon_filter_test(filteractionplaysoundtest
    filteractionplaysoundtest.cpp
    ../filteractions/filteractionplaysound.cpp
    ../filteractions/filteractionwithtest.cpp
    ../filteractions/filteractionplaysound.cpp
    ../soundtestwidget.cpp
    ../dialog/filteractionmissingsoundurldialog.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(invalidfilterdialogtest
    invalidfilterdialogtest.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(invalidfilterwidgettest
    invalidfilterwidgettest.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(invalidfilterlistwidgettest
    invalidfilterlistwidgettest.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionunsetstatustest
    filteractionunsetstatustest.cpp
    ../filteractions/filteractionunsetstatus.cpp
    ../filteractions/filteractionstatus.cpp
    ../filteractions/filteractionwithstringlist.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionredirecttest
    filteractionredirecttest.cpp
    ../filteractions/filteractionredirect.cpp
    ../filteractions/filteractionwithaddress.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(invalidfilterinfotest
    invalidfilterinfotest.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionsendfakedispositiontest
    filteractionsendfakedispositiontest.cpp
    ../filteractions/filteractionsendfakedisposition.cpp
    ../filteractions/filteractionwithstringlist.cpp
    ../filteractions/filteractionwithstring.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionforwardtest
    filteractionforwardtest.cpp
    ../filteractions/filteractionforward.cpp
    ../filteractions/filteractionwithaddress.cpp
    ../filteractions/filteractionwithstring.cpp
    ../dialog/filteractionmissingtemplatedialog.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(invalidfilterinfowidgettest
    invalidfilterinfowidgettest.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractioncopytest
    filteractioncopytest.cpp
    ../filteractions/filteractioncopy.cpp
    ../filteractions/filteractionwithfolder.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionmovetest
    filteractionmovetest.cpp
    ../filteractions/filteractionmove.cpp
    ../filteractions/filteractionwithfolder.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionexectest
    filteractionexectest.cpp
    ../filteractions/filteractionexec.cpp
    ../filteractions/filteractionwithcommand.cpp
    ../filteractions/filteractionwithurl.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionsendreceipttest
    filteractionsendreceipttest.cpp
    ../filteractions/filteractionsendreceipt.cpp
    ../filteractions/filteractionwithnone.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionsettransporttest
    filteractionsettransporttest.cpp
    ../filteractions/filteractionsettransport.cpp
    ../dialog/filteractionmissingtransportdialog.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filterconverttosieveresultdialog
    filterconverttosieveresultdialogtest.cpp
    ../filterconverter/filterconverttosieveresultdialog.cpp
    ../filterconverter/filterconverttosievepurposemenuwidget.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(mailfiltertest
    mailfiltertest.cpp
    ${filter_common_SRCS}
)

add_mailcommon_filter_test(filteractionmissingsoundurldialogtest
    filteractionmissingsoundurldialogtest.cpp
    ../dialog/filteractionmissingsoundurldialog.cpp
    )

add_mailcommon_filter_test(filteractionmissingtemplatedialogtest
    filteractionmissingtemplatedialogtest.cpp
    ../dialog/filteractionmissingtemplatedialog.cpp
    )

add_mailcommon_filter_test(filteractionmissingtransportdialogtest
    filteractionmissingtransportdialogtest.cpp
    ../dialog/filteractionmissingtransportdialog.cpp
    )

add_mailcommon_filter_test(filteractionmissingaccountdialogtest
    filteractionmissingaccountdialogtest.cpp
    ../dialog/filteractionmissingaccountdialog.cpp
    )

add_mailcommon_filter_test(filteractionmissingtagdialogtest
    filteractionmissingtagdialogtest.cpp
    ../dialog/filteractionmissingtagdialog.cpp
    )

add_mailcommon_filter_test(filteractionmissingidentitydialogtest
    filteractionmissingidentitydialogtest.cpp
    ../dialog/filteractionmissingidentitydialog.cpp
    ../../../autotests/dummykernel.cpp
    )

add_mailcommon_filter_test(filterimporterpathcachetest
    filterimporterpathcachetest.cpp
    )

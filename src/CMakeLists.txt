# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
add_definitions(-DTRANSLATION_DOMAIN=\"libmailcommon\")

#add_definitions( -DQT_NO_CAST_FROM_ASCII )
#add_definitions( -DQT_NO_CAST_TO_ASCII )

if(BUILD_TESTING)
   add_definitions(-DBUILD_TESTING)
endif()



kconfig_add_kcfg_files(libmailcommon_SRCS
    settings/mailcommonsettings_base.kcfgc
)


ecm_qt_declare_logging_category(libmailcommon_SRCS HEADER mailcommon_debug.h IDENTIFIER MAILCOMMON_LOG CATEGORY_NAME org.kde.pim.mailcommon
        OLD_CATEGORY_NAMES log_mailcommon
        DESCRIPTION "mailcommon (mailcommon)" EXPORT MAILCOMMON)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/dbusinterfaces/org.freedesktop.Akonadi.MailFilterAgent.xml PROPERTIES INCLUDE "dbusoperators.h")

qt_add_dbus_interfaces(libmailcommon_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/dbusinterfaces/org.freedesktop.Akonadi.MailFilterAgent.xml
)

ki18n_wrap_ui(libmailcommon_SRCS filter/ui/filterconfigwidget.ui snippets/ui/snippetwidget.ui filter/ui/selectthunderbirdfilterfileswidget.ui)

set(libmailcommon_SRCS ${libmailcommon_SRCS} filter/soundtestwidget.cpp)

macro(add_resource_iface _kcfgFile _ifaceName _className)
  kcfg_generate_dbus_interface(${CMAKE_CURRENT_SOURCE_DIR}/${_kcfgFile} ${_ifaceName})
  qt_add_dbus_interface(libmailcommon_SRCS
    ${CMAKE_CURRENT_BINARY_DIR}/${_ifaceName}.xml ${_className}
  )
endmacro()

add_resource_iface(settings.kcfg org.kde.Akonadi.POP3.Settings pop3settings)

qt_add_dbus_interfaces(libmailcommon_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/dbusinterfaces/org.kde.Korganizer.Calendar.xml
)

add_library(KF5MailCommon)

add_library(KF5::MailCommon ALIAS KF5MailCommon)

target_sources(KF5MailCommon PRIVATE
    filter/kmfilteraccountlist.cpp
    filter/kmfilterlistbox.cpp
    filter/filterselectiondialog.cpp
    filter/filterconverter/filterconverttosieve.cpp
    filter/filterconverter/filterconverttosieveresultdialog.cpp
    filter/filterconverter/filterconverttosievepurposemenuwidget.cpp
    filter/filteractions/filteraction.cpp
    filter/filteractions/filteractionaddheader.cpp
    filter/filteractions/filteractionaddtag.cpp
    filter/filteractions/filteractionaddtoaddressbook.cpp
    filter/filteractions/filteractioncopy.cpp
    filter/filteractions/filteractiondecrypt.cpp
    filter/filteractions/filteractiondelete.cpp
    filter/filteractions/filteractiondict.cpp
    filter/filteractions/filteractionencrypt.cpp
    filter/filteractions/filteractionexec.cpp
    filter/filteractions/filteractionforward.cpp
    filter/filteractions/filteractionmove.cpp
    filter/filteractions/filteractionpipethrough.cpp
    filter/filteractions/filteractionplaysound.cpp
    filter/filteractions/filteractionredirect.cpp
    filter/filteractions/filteractionremoveheader.cpp
    filter/filteractions/filteractionreplyto.cpp
    filter/filteractions/filteractionrewriteheader.cpp
    filter/filteractions/filteractionsendfakedisposition.cpp
    filter/filteractions/filteractionsendreceipt.cpp
    filter/filteractions/filteractionsetidentity.cpp
    filter/filteractions/filteractionsetstatus.cpp
    filter/filteractions/filteractionstatus.cpp
    filter/filteractions/filteractionunsetstatus.cpp
    filter/filteractions/filteractionsettransport.cpp
    filter/filteractions/filteractionwidget.cpp
    filter/filteractions/filteractionwithaddress.cpp
    filter/filteractions/filteractionwithcommand.cpp
    filter/filteractions/filteractionwithcrypto.cpp
    filter/filteractions/filteractionwithfolder.cpp
    filter/filteractions/filteractionwithnone.cpp
    filter/filteractions/filteractionwithstring.cpp
    filter/filteractions/filteractionwithstringlist.cpp
    filter/filteractions/filteractionwithtest.cpp
    filter/filteractions/filteractionwithuoid.cpp
    filter/filteractions/filteractionwithurl.cpp
    filter/filterimporterexporter.cpp
    filter/filterimporter/filterimporterabstract.cpp
    filter/filterimporter/filterimporterevolution.cpp
    filter/filterimporter/filterimportersylpheed.cpp
    filter/filterimporter/filterimporterthunderbird.cpp
    filter/filterimporter/filterimporterprocmail.cpp
    filter/filterimporter/filterimporterbalsa.cpp
    filter/filterimporter/filterimporterclawsmail.cpp
    filter/filterimporter/filterimportergmail.cpp
    filter/filterlog.cpp
    filter/filtermanager.cpp
    filter/itemcontext.cpp
    filter/kmfilterdialog.cpp
    filter/mailfilter.cpp
    filter/filterimporterpathcache.cpp
    filter/dialog/selectthunderbirdfilterfilesdialog.cpp
    filter/dialog/selectthunderbirdfilterfileswidget.cpp
    filter/dialog/filteractionmissingfolderdialog.cpp
    filter/dialog/filteractionmissingsoundurldialog.cpp
    filter/dialog/filteractionmissingtagdialog.cpp
    filter/dialog/filteractionmissingaccountdialog.cpp
    filter/dialog/filteractionmissingtemplatedialog.cpp
    filter/dialog/filteractionmissingtransportdialog.cpp
    filter/dialog/filteractionmissingidentitydialog.cpp
    filter/invalidfilters/invalidfilterdialog.cpp
    filter/invalidfilters/invalidfilterlistview.cpp
    filter/invalidfilters/invalidfilterwidget.cpp
    filter/invalidfilters/invalidfilterinfo.cpp
    filter/invalidfilters/invalidfilterlistitemdelegate.cpp
    filter/invalidfilters/invalidfilterlistmodel.cpp
    filter/invalidfilters/invalidfilterinfowidget.cpp
    collectionpage/attributeregistrar.cpp
    collectionpage/collectiongeneralpage.cpp
    collectionpage/collectionexpirypage.cpp
    collectionpage/collectionexpirywidget.cpp
    collectionpage/attributes/expirecollectionattribute.cpp
    collectionpage/collectionexpiryjob.cpp
    collectionpage/collectiontemplateswidget.cpp
    collectionpage/collectionviewwidget.cpp
    collectionpage/collectiongeneralwidget.cpp
    folder/foldersettings.cpp
    folder/foldercollectionmonitor.cpp
    folder/folderrequester.cpp
    folder/folderselectiondialog.cpp
    folder/foldertreeview.cpp
    folder/foldertreewidget.cpp
    folder/foldertreewidgetproxymodel.cpp
    folder/entitycollectionorderproxymodel.cpp
    folder/accountconfigorderdialog.cpp
    folder/favoritecollectionorderproxymodel.cpp
    folder/hierarchicalfoldermatcher.cpp
    job/jobscheduler.cpp
    job/folderjob.cpp
    job/expirejob.cpp
    job/backupjob.cpp
    search/widgethandler/rulewidgethandlermanager.cpp
    search/searchpattern.cpp
    search/searchpatternedit.cpp
    search/widgethandler/encryptionwidgethandler.cpp
    search/widgethandler/textrulerwidgethandler.cpp
    search/widgethandler/statusrulewidgethandler.cpp
    search/widgethandler/messagerulewidgethandler.cpp
    search/widgethandler/tagrulewidgethandler.cpp
    search/widgethandler/numericrulewidgethandler.cpp
    search/widgethandler/daterulewidgethandler.cpp
    search/widgethandler/numericdoublerulewidgethandler.cpp
    search/widgethandler/headersrulerwidgethandler.cpp
    search/searchrule/searchrulenumerical.cpp
    search/searchrule/searchruledate.cpp
    search/searchrule/searchrulestring.cpp
    search/searchrule/searchrulestatus.cpp
    search/searchrule/searchruleencryption.cpp
    search/searchrule/searchrule.cpp
    snippets/snippetdialog.cpp
    snippets/snippetsmanager.cpp
    snippets/snippetsmodel.cpp
    snippets/snippetvariabledialog.cpp
    snippets/snippettreeview.cpp
    snippets/snippetwidget.cpp
    snippets/snippetattachmentwidget.cpp
    snippets/snippetselectattachmentwidget.cpp
    snippets/snippetselectattachmentdialog.cpp
    snippets/snippetcustomfileattachmentnamewidget.cpp
    snippets/snippetcustomfileattachmentnamedialog.cpp
    tag/tagwidget.cpp
    tag/tag.cpp
    tag/addtagdialog.cpp
    widgets/redirectdialog.cpp
    widgets/redirectwidget.cpp
    widgets/favoritecollectionwidget.cpp
    mdn/mdnwarningjob.h
    mdn/mdnwarningjob.cpp
    util/cryptoutils.cpp
    util/mailutil.cpp
    util/resourcereadconfigfile.cpp
    ${libmailcommon_SRCS}
    kernel/mailkernel.cpp
    job/folderjob.h
    job/expirejob.h
    job/jobscheduler.h
    job/backupjob.h
    filter/filterselectiondialog.h
    filter/kmfilteraccountlist.h
    filter/filterlog.h
    filter/dialog/filteractionmissingidentitydialog.h
    filter/dialog/filteractionmissingtemplatedialog.h
    filter/dialog/filteractionmissingaccountdialog.h
    filter/dialog/filteractionmissingtransportdialog.h
    filter/dialog/filteractionmissingfolderdialog.h
    filter/dialog/selectthunderbirdfilterfilesdialog.h
    filter/dialog/filteractionmissingsoundurldialog.h
    filter/dialog/filteractionmissingtagdialog.h
    filter/dialog/selectthunderbirdfilterfileswidget.h
    filter/invalidfilters/invalidfilterinfo.h
    filter/invalidfilters/invalidfilterlistitemdelegate.h
    filter/invalidfilters/invalidfilterlistmodel.h
    filter/invalidfilters/invalidfilterinfowidget.h
    filter/invalidfilters/invalidfilterdialog.h
    filter/invalidfilters/invalidfilterlistview.h
    filter/invalidfilters/invalidfilterwidget.h
    filter/kmfilterdialog.h
    filter/filtermanager.h
    filter/filterimporter/filterimportersylpheed.h
    filter/filterimporter/filterimportergmail.h
    filter/filterimporter/filterimporterprocmail.h
    filter/filterimporter/filterimporterabstract.h
    filter/filterimporter/filterimporterclawsmail.h
    filter/filterimporter/filterimporterthunderbird.h
    filter/filterimporter/filterimporterbalsa.h
    filter/filterimporter/filterimporterevolution.h
    filter/kmfilterlistbox.h
    filter/filterimporterpathcache.h
    filter/mailfilter.h
    filter/filteractions/filteractionexec.h
    filter/filteractions/filteractionstatus.h
    filter/filteractions/filteractionredirect.h
    filter/filteractions/filteractionwithaddress.h
    filter/filteractions/filteractionunsetstatus.h
    filter/filteractions/filteractiondict.h
    filter/filteractions/filteractionwithstring.h
    filter/filteractions/filteractionsettransport.h
    filter/filteractions/filteractionaddtag.h
    filter/filteractions/filteractionaddtoaddressbook.h
    filter/filteractions/filteractionwithcrypto.h
    filter/filteractions/filteractionreplyto.h
    filter/filteractions/filteractionwithnone.h
    filter/filteractions/filteractionwithurl.h
    filter/filteractions/filteractionpipethrough.h
    filter/filteractions/filteractiondelete.h
    filter/filteractions/filteractionwithtest.h
    filter/filteractions/filteractionwithuoid.h
    filter/filteractions/filteractionplaysound.h
    filter/filteractions/filteractionsetstatus.h
    filter/filteractions/filteractionwithstringlist.h
    filter/filteractions/filteraction.h
    filter/filteractions/filteractionsendreceipt.h
    filter/filteractions/filteractionsendfakedisposition.h
    filter/filteractions/filteractionremoveheader.h
    filter/filteractions/filteractioncopy.h
    filter/filteractions/filteractionsetidentity.h
    filter/filteractions/filteractiondecrypt.h
    filter/filteractions/filteractionencrypt.h
    filter/filteractions/filteractionmove.h
    filter/filteractions/filteractionforward.h
    filter/filteractions/filteractionwidget.h
    filter/filteractions/filteractionrewriteheader.h
    filter/filteractions/filteractionwithfolder.h
    filter/filteractions/filteractionaddheader.h
    filter/filteractions/filteractionwithcommand.h
    filter/itemcontext.h
    filter/filterimporterexporter.h
    filter/soundtestwidget.h
    filter/filterconverter/filterconverttosievepurposemenuwidget.h
    filter/filterconverter/filterconverttosieve.h
    filter/filterconverter/filterconverttosieveresultdialog.h
    search/widgethandler/textrulerwidgethandler.h
    search/widgethandler/rulewidgethandlermanager.h
    search/widgethandler/daterulewidgethandler.h
    search/widgethandler/encryptionwidgethandler.h
    search/widgethandler/numericdoublerulewidgethandler.h
    search/widgethandler/tagrulewidgethandler.h
    search/widgethandler/headersrulerwidgethandler.h
    search/widgethandler/messagerulewidgethandler.h
    search/widgethandler/statusrulewidgethandler.h
    search/widgethandler/numericrulewidgethandler.h
    search/searchrule/searchruledate.h
    search/searchrule/searchrule.h
    search/searchrule/searchrulestatus.h
    search/searchrule/searchrulenumerical.h
    search/searchrule/searchruleencryption.h
    search/searchrule/searchrulestring.h
    search/searchpattern.h
    search/searchpatternedit.h
    folder/entitycollectionorderproxymodel.h
    folder/foldercollectionmonitor.h
    folder/foldersettings.h
    folder/hierarchicalfoldermatcher_p.h
    folder/foldertreewidget.h
    folder/foldertreeview.h
    folder/accountconfigorderdialog.h
    folder/foldertreewidgetproxymodel.h
    folder/folderselectiondialog.h
    folder/folderrequester.h
    folder/favoritecollectionorderproxymodel.h
    mailcommon_private_export.h
    kernel/mailkernel.h
    snippets/snippetsmanager.h
    snippets/snippetselectattachmentwidget.h
    snippets/snippetattachmentwidget.h
    snippets/snippetdialog.h
    snippets/snippetcustomfileattachmentnamedialog.h
    snippets/snippetcustomfileattachmentnamewidget.h
    snippets/snippettreeview.h
    snippets/snippetvariabledialog.h
    snippets/snippetwidget.h
    snippets/snippetselectattachmentdialog.h
    snippets/snippetsmodel.h
    interfaces/mailinterfaces.h
    interfaces/rulewidgethandler.h
    tag/tag.h
    tag/addtagdialog.h
    tag/tagwidget.h
    collectionpage/collectionexpirywidget.h
    collectionpage/collectiongeneralpage.h
    collectionpage/collectionexpiryjob.h
    collectionpage/collectionexpirypage.h
    collectionpage/collectiongeneralwidget.h
    collectionpage/collectionviewwidget.h
    collectionpage/collectiontemplateswidget.h
    collectionpage/attributes/expirecollectionattribute.h
    widgets/redirectwidget.h
    widgets/redirectdialog.h
    widgets/favoritecollectionwidget.h
    dbusoperators.h
    util/mailutil_p.h
    util/mailutil.h
    util/cryptoutils.h
    util/resourcereadconfigfile.h
    )


if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF5MailCommon PROPERTIES UNITY_BUILD ON)
    set_source_files_properties(${libmailcommon_search_SRCS} PROPERTIES SKIP_UNITY_BUILD_INCLUSION ON)
endif()
generate_export_header(KF5MailCommon BASE_NAME mailcommon)


target_link_libraries(KF5MailCommon
PUBLIC
  KF5::AkonadiCore
  KF5::AkonadiMime
  KF5::Libkdepim
  KF5::MessageComposer
  KF5::PimCommonAkonadi
  KF${KF_MAJOR_VERSION}::Completion
PRIVATE
  Gpgmepp
  KF5::AkonadiWidgets
  KF${KF_MAJOR_VERSION}::TextWidgets
  KF${KF_MAJOR_VERSION}::I18n
  KF5::MessageViewer
  KF5::MailImporter
  KF5::MessageCore
  KF5::TemplateParser
  KF5::MessageList
  KF5::Mime
  KF5::MailTransport
  Phonon::phonon4qt${QT_MAJOR_VERSION}
  KF${KF_MAJOR_VERSION}::XmlGui
  KF${KF_MAJOR_VERSION}::KIOWidgets
  KF${KF_MAJOR_VERSION}::IconThemes
  KF${KF_MAJOR_VERSION}::Archive
  KF${KF_MAJOR_VERSION}::ItemViews
  KF${KF_MAJOR_VERSION}::SyntaxHighlighting
  KF${KF_MAJOR_VERSION}::GuiAddons
  KF${KF_MAJOR_VERSION}::Codecs
  Qt::Xml
)

if (QT_MAJOR_VERSION STREQUAL "6")
    target_link_libraries(KF5MailCommon PRIVATE QGpgmeQt6)
else()
    target_link_libraries(KF5MailCommon PRIVATE QGpgme)
endif()


target_include_directories(KF5MailCommon INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/MailCommon/>")
target_include_directories(KF5MailCommon PUBLIC "$<BUILD_INTERFACE:${mailcommon_SOURCE_DIR}/src;${mailcommon_BINARY_DIR}/src;${mailcommon_SOURCE_DIR}/src/search;${mailcommon_SOURCE_DIR}/src/folder>")


set_target_properties(KF5MailCommon PROPERTIES
    VERSION ${MAILCOMMON_VERSION}
    SOVERSION ${MAILCOMMON_SOVERSION}
    EXPORT_NAME MailCommon
)

install(TARGETS
    KF5MailCommon
    EXPORT KF5MailCommonTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS}
)

if (BUILD_TESTING)
   add_subdirectory(filter/filterimporter/autotests)
   add_subdirectory(collectionpage/autotests)
   add_subdirectory(filter/autotests)
   add_subdirectory(snippets/autotests)
   add_subdirectory(filter/tests) 
   add_subdirectory(search/autotests)
endif()

ecm_generate_headers(MailCommon_CamelCase_HEADERS
  HEADER_NAMES
  CryptoUtils
  MailUtil
  ResourceReadConfigFile
  REQUIRED_HEADERS MailCommon_HEADERS
  PREFIX MailCommon
  RELATIVE util
)

ecm_generate_headers(MailCommon_Camelcasetop_HEADERS
  HEADER_NAMES
  DBusOperators
  REQUIRED_HEADERS MailCommon_top_HEADERS
  PREFIX MailCommon
)

ecm_generate_headers(MailCommon_Camelcasetag_HEADERS
  HEADER_NAMES
  TagWidget
  Tag
  AddTagDialog
  REQUIRED_HEADERS MailCommon_tag_HEADERS
  PREFIX MailCommon
  RELATIVE tag
)

ecm_generate_headers(MailCommon_Camelcasefolder_HEADERS
  HEADER_NAMES
  FolderSettings
  FolderRequester
  FolderCollectionMonitor
  AccountConfigOrderDialog
  FolderTreeView
  FolderTreeWidget
  FolderSelectionDialog
  FolderTreeWidgetProxyModel
  FavoriteCollectionOrderProxyModel
  REQUIRED_HEADERS MailCommon_folder_HEADERS
  PREFIX MailCommon
  RELATIVE folder
)

ecm_generate_headers(MailCommon_Camelcasekernel_HEADERS
  HEADER_NAMES
  MailKernel
  REQUIRED_HEADERS MailCommon_kernel_HEADERS
  PREFIX MailCommon
  RELATIVE kernel
)

ecm_generate_headers(MailCommon_Camelcaseinterfaces_HEADERS
  HEADER_NAMES
  MailInterfaces
  REQUIRED_HEADERS MailCommon_interfaces_HEADERS
  PREFIX MailCommon
  RELATIVE interfaces
)


ecm_generate_headers(MailCommon_Camelcasefilter_HEADERS
  HEADER_NAMES
  FilterLog
  MailFilter
  FilterImporterExporter
  FilterManager
  KMFilterDialog
  FilterImporterPathCache
  ItemContext
  REQUIRED_HEADERS MailCommon_filter_HEADERS
  PREFIX MailCommon
  RELATIVE filter
)

ecm_generate_headers(MailCommon_Camelcasejob_HEADERS
  HEADER_NAMES
  BackupJob
  JobScheduler
  FolderJob
  REQUIRED_HEADERS MailCommon_job_HEADERS
  PREFIX MailCommon
  RELATIVE job
)

ecm_generate_headers(MailCommon_Camelcasefilteraction_HEADERS
  HEADER_NAMES
  FilterAction
  FilterActionDict
  REQUIRED_HEADERS MailCommon_filteraction_HEADERS
  PREFIX MailCommon
  RELATIVE filter/filteractions/
)

ecm_generate_headers(MailCommon_Camelcasemdn_HEADERS
  HEADER_NAMES
  MDNWarningJob
  REQUIRED_HEADERS MailCommon_mdn_HEADERS
  PREFIX MailCommon
  RELATIVE mdn
)


ecm_generate_headers(MailCommon_Camelcasecollectionpage_HEADERS
  HEADER_NAMES
  CollectionGeneralPage
  CollectionExpiryPage
  CollectionExpiryWidget
  CollectionTemplatesWidget
  CollectionViewWidget
  CollectionGeneralWidget
  REQUIRED_HEADERS MailCommon_collectionpage_HEADERS
  PREFIX MailCommon
  RELATIVE collectionpage
)

ecm_generate_headers(MailCommon_Camelcasecollectionpageattributes_HEADERS
  HEADER_NAMES
  ExpireCollectionAttribute
  REQUIRED_HEADERS MailCommon_collectionpageattributes_HEADERS
  PREFIX MailCommon
  RELATIVE collectionpage/attributes
)

ecm_generate_headers(MailCommon_Camelcasefilterimporter_HEADERS
  HEADER_NAMES
  FilterImporterBalsa
  FilterImporterClawsMail
  FilterImporterAbstract
  REQUIRED_HEADERS MailCommon_filterimporter_HEADERS
  PREFIX MailCommon
  RELATIVE filter/filterimporter/
)

ecm_generate_headers(MailCommon_Camelcasesnippets_HEADERS
  HEADER_NAMES
  SnippetsManager
  SnippetTreeView
  SnippetsModel
  SnippetWidget
  REQUIRED_HEADERS MailCommon_snippets_HEADERS
  PREFIX MailCommon
  RELATIVE snippets
)

ecm_generate_headers(MailCommon_Camelcasekernel_HEADERS
  HEADER_NAMES
  SearchPattern
  SearchPatternEdit
  REQUIRED_HEADERS MailCommon_kernel_HEADERS
  PREFIX MailCommon
  RELATIVE search
)

ecm_generate_headers(MailCommon_Camelcasewidgets_HEADERS
  HEADER_NAMES
  RedirectDialog
  FavoriteCollectionWidget
  REQUIRED_HEADERS MailCommon_widgets_HEADERS
  PREFIX MailCommon
  RELATIVE widgets
)

ecm_generate_headers(MailCommon_Camelcasesearchrule_HEADERS
  HEADER_NAMES
  SearchRule
  SearchRuleStatus
  REQUIRED_HEADERS MailCommon_searchrule_HEADERS
  PREFIX MailCommon
  RELATIVE search/searchrule
)

ecm_generate_pri_file(BASE_NAME MailCommon
    LIB_NAME KF5MailCommon
    DEPS "AkonadiCore AkonadiMime MessageComposer PimCommon" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF}/MailCommon
)

install(FILES
    ${MailCommon_Camelcasesearchrule_HEADERS}
    ${MailCommon_CamelCase_HEADERS}
    ${MailCommon_Camelcasetop_HEADERS}
    ${MailCommon_Camelcasetag_HEADERS}
    ${MailCommon_Camelcasefolder_HEADERS}
    ${MailCommon_Camelcasekernel_HEADERS}
    ${MailCommon_Camelcaseinterfaces_HEADERS}
    ${MailCommon_Camelcasefilter_HEADERS}
    ${MailCommon_Camelcasejob_HEADERS}
    ${MailCommon_Camelcasefilteraction_HEADERS}
    ${MailCommon_Camelcasemdn_HEADERS}
    ${MailCommon_Camelcasecollectionpage_HEADERS}
    ${MailCommon_Camelcasecollectionpageattributes_HEADERS}
    ${MailCommon_Camelcasefilterimporter_HEADERS}
    ${MailCommon_Camelcasesnippets_HEADERS}
    ${MailCommon_Camelcasewidgets_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/MailCommon/MailCommon
    COMPONENT Devel
)

install(FILES
    ${MailCommon_searchrule_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/mailcommon_export.h
    ${CMAKE_CURRENT_BINARY_DIR}/pop3settings.h
    ${CMAKE_CURRENT_BINARY_DIR}/mailcommonsettings_base.h
    ${MailCommon_HEADERS}
    ${MailCommon_top_HEADERS}
    ${MailCommon_tag_HEADERS}
    ${MailCommon_folder_HEADERS}
    ${MailCommon_kernel_HEADERS}
    ${MailCommon_interfaces_HEADERS}
    ${MailCommon_filter_HEADERS}
    ${MailCommon_job_HEADERS}
    ${MailCommon_filteraction_HEADERS}
    ${MailCommon_mdn_HEADERS}
    ${MailCommon_collectionpage_HEADERS}
    ${MailCommon_collectionpageattributes_HEADERS}
    ${MailCommon_filterimporter_HEADERS}
    ${MailCommon_snippets_HEADERS}
    ${MailCommon_widgets_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/MailCommon/mailcommon
    COMPONENT Devel
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/mailcommon_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/MailCommon
    COMPONENT Devel
)

install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

if(BUILD_DESIGNERPLUGIN)
    add_subdirectory(designer)
endif()

if (BUILD_QCH)
    ecm_add_qch(
        KF5MailCommon_QCH
        NAME KF5MailCommon
        BASE_NAME KF5MailCommon
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${MailCommon_HEADERS}
            ${MailCommon_top_HEADERS}
            ${MailCommon_tag_HEADERS}
            ${MailCommon_folder_HEADERS}
            ${MailCommon_kernel_HEADERS}
            ${MailCommon_interfaces_HEADERS}
            ${MailCommon_filter_HEADERS}
            ${MailCommon_job_HEADERS}
            ${MailCommon_filteraction_HEADERS}
            ${MailCommon_mdn_HEADERS}
            ${MailCommon_collectionpage_HEADERS}
            ${MailCommon_collectionpageattributes_HEADERS}
            ${MailCommon_filterimporter_HEADERS}
            ${MailCommon_snippets_HEADERS}
            ${MailCommon_widgets_HEADERS}
        #MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt${QT_MAJOR_VERSION}Core_QCH
            Qt${QT_MAJOR_VERSION}Gui_QCH
            Qt${QT_MAJOR_VERSION}Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            MAILCOMMON_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

ecm_qt_install_logging_categories(EXPORT MAILCOMMON FILE mailcommon.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})

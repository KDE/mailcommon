# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(PIM_VERSION "5.24.0")

project(mailcommon VERSION ${PIM_VERSION})

set(KF_MIN_VERSION "5.105.0")

find_package(ECM ${KF_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)

include(ECMSetupVersion)
include(FeatureSummary)
include(KDEGitCommitHooks)
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)
include(ECMAddTests)
include(ECMAddQch)

if (QT_MAJOR_VERSION STREQUAL "6")
    set(QT_REQUIRED_VERSION "6.4.0")
    set(KF_MIN_VERSION "5.240.0")
    set(KF_MAJOR_VERSION "6")
else()
    set(KF_MAJOR_VERSION "5")
endif()

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")
option(BUILD_WITH_COMPAT_LIBS "Generate compat cmake file (Set at false will allow to co-install lib)" ON)

set(QT_REQUIRED_VERSION "5.15.2")
if (QT_MAJOR_VERSION STREQUAL "6")
    set(QT_REQUIRED_VERSION "6.4.0")
endif()

set(MAILCOMMON_LIB_VERSION ${PIM_VERSION})
set(AKONADIMIME_LIB_VERSION "5.24.0")
set(AKONADICONTACT_LIB_VERSION "5.24.0")
set(MESSAGELIB_LIB_VERSION "5.24.0")
set(KMIME_LIB_VERSION "5.24.0")
set(KMAILTRANSPORT_LIB_VERSION "5.24.0")
set(MAILIMPORTER_LIB_VERSION "5.24.0")
set(LIBKDEPIM_LIB_VERSION "5.24.0")
set(PIMCOMMON_LIB_VERSION "5.24.0")

set(PHONON_LIB_VERSION "4.11.1")
set(AKONADI_VERSION "5.24.0")

find_package(Qt${QT_MAJOR_VERSION} ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets DBus Test Xml)

set(GPGMEPP_LIB_VERSION "1.16.0")
find_package(Gpgmepp ${GPGMEPP_LIB_VERSION} CONFIG REQUIRED)
if (QT_MAJOR_VERSION STREQUAL "6")
    find_package(QGpgmeQt6 ${GPGME_REQUIRED_VERSION} CONFIG REQUIRED)
    find_package(Qt6Core5Compat)
else()
    find_package(QGpgme ${GPGME_REQUIRED_VERSION} CONFIG REQUIRED)
endif()

find_package(KF${KF_MAJOR_VERSION}Archive ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}Completion ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}Config ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}I18n ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}IconThemes ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}ItemModels ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}ItemViews ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}KIO ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}TextWidgets ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}WidgetsAddons ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}XmlGui ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}DBusAddons ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}SyntaxHighlighting ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}GuiAddons ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}Codecs ${KF_MIN_VERSION} CONFIG REQUIRED)

find_package(KPim${KF_MAJOR_VERSION}Akonadi ${AKONADI_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}AkonadiMime ${AKONADIMIME_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}Libkdepim ${LIBKDEPIM_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}MailImporter ${MAILIMPORTER_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}MailTransport ${KMAILTRANSPORT_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}MessageComposer ${MESSAGELIB_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}MessageList ${MESSAGELIB_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}MessageCore ${MESSAGELIB_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}MessageViewer ${MESSAGELIB_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}Mime ${KMIME_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}PimCommonAkonadi ${PIMCOMMON_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}TemplateParser ${MESSAGELIB_LIB_VERSION} CONFIG REQUIRED)
find_package(Phonon4Qt${QT_MAJOR_VERSION} ${PHONON_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}ContactEditor ${AKONADICONTACT_LIB_VERSION} CONFIG REQUIRED)

option(BUILD_DESIGNERPLUGIN "Build plugin for Qt Designer" ON)
add_feature_info(DESIGNERPLUGIN ${BUILD_DESIGNERPLUGIN} "Build plugin for Qt Designer")

ecm_setup_version(PROJECT VARIABLE_PREFIX MAILCOMMON
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/mailcommon_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}MailCommonConfigVersion.cmake"
                        SOVERSION 5
)


########### Targets ###########
remove_definitions( -DQT_NO_CAST_FROM_ASCII )

option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if (USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT ON)
endif()


ecm_set_disabled_deprecation_versions(QT 6.4  KF 5.105.0)

add_subdirectory(src)
if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()

########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim${KF_MAJOR_VERSION}MailCommon")
set(MAILCOMMON_KF5_COMPAT FALSE)
if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KPim${KF_MAJOR_VERSION}MailCommon_QCH
        FILE KPim${KF_MAJOR_VERSION}MailCommonQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KPim${KF_MAJOR_VERSION}MailCommonQchTargets.cmake\")")
endif()

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KPimMailCommonConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}MailCommonConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}MailCommonConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}MailCommonConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KPim${KF_MAJOR_VERSION}MailCommonTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KPim${KF_MAJOR_VERSION}MailCommonTargets.cmake NAMESPACE KPim${KF_MAJOR_VERSION}::)

install(FILES
   ${CMAKE_CURRENT_BINARY_DIR}/mailcommon_version.h
  DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/MailCommon COMPONENT Devel
)

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ki18n_install(po)

if (BUILD_WITH_COMPAT_LIBS)
if (QT_MAJOR_VERSION STREQUAL "5")
##
# TODO: Backwards compatiblity. Remove in next major version
##
set(CMAKECONFIG_INSTALL_DIR_KF5 "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5MailCommon")
set(MAILCOMMON_KF5_COMPAT TRUE)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimMailCommonConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5MailCommonConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR_KF5}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5MailCommonConfig.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
    COMPONENT Devel
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}MailCommonConfigVersion.cmake"
    RENAME "KF5MailCommonConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
    COMPONENT Devel
)
install(EXPORT KPim${KF_MAJOR_VERSION}MailCommonTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
    FILE KPim${KF_MAJOR_VERSION}MailCommonTargets.cmake
    NAMESPACE KF5::
)

if (BUILD_QCH)
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}MailCommonQchTargets.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
        COMPONENT Devel
    )
endif()
endif()
endif()

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

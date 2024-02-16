# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(PIM_VERSION "6.0.40")

project(mailcommon VERSION ${PIM_VERSION})

set(KF_MIN_VERSION "5.248.0")
set(QT_REQUIRED_VERSION "6.6.0")
find_package(ECM ${KF_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(GenerateExportHeader)
include(ECMGenerateHeaders)

include(ECMSetupVersion)
include(FeatureSummary)
include(KDEGitCommitHooks)
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)
include(ECMFeatureSummary)
include(ECMAddTests)
include(ECMAddQch)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

set(MAILCOMMON_LIB_VERSION ${PIM_VERSION})
set(AKONADIMIME_LIB_VERSION "6.0.40")
set(AKONADICONTACT_LIB_VERSION "6.0.40")
set(MESSAGELIB_LIB_VERSION "6.0.40")
set(KMIME_LIB_VERSION "6.0.40")
set(KMAILTRANSPORT_LIB_VERSION "6.0.40")
set(MAILIMPORTER_LIB_VERSION "6.0.40")
set(LIBKDEPIM_LIB_VERSION "6.0.40")
set(PIMCOMMON_LIB_VERSION "6.0.40")

set(PHONON_LIB_VERSION "4.11.1")
set(AKONADI_VERSION "6.0.40")
set(KTEXTADDONS_MIN_VERSION "1.5.3")

find_package(Qt6 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets DBus Xml)

set(GPGMEPP_LIB_VERSION "1.20.0")
find_package(Gpgmepp ${GPGMEPP_LIB_VERSION} CONFIG REQUIRED)
find_package(QGpgmeQt6 ${GPGMEPP_LIB_VERSION} CONFIG REQUIRED)

find_package(KF6Archive ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6Completion ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6Config ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6I18n ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6IconThemes ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6ItemModels ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6ItemViews ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6KIO ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6TextWidgets ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6WidgetsAddons ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6XmlGui ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6DBusAddons ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6SyntaxHighlighting ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6GuiAddons ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6Codecs ${KF_MIN_VERSION} CONFIG REQUIRED)

find_package(KPim6Akonadi ${AKONADI_VERSION} CONFIG REQUIRED)
find_package(KPim6AkonadiMime ${AKONADIMIME_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6Libkdepim ${LIBKDEPIM_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6MailImporter ${MAILIMPORTER_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6MailTransport ${KMAILTRANSPORT_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6MessageComposer ${MESSAGELIB_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6MessageList ${MESSAGELIB_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6MessageCore ${MESSAGELIB_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6MessageViewer ${MESSAGELIB_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6Mime ${KMIME_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6PimCommonAkonadi ${PIMCOMMON_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6TemplateParser ${MESSAGELIB_LIB_VERSION} CONFIG REQUIRED)
find_package(Phonon4Qt6 ${PHONON_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6AkonadiContactWidgets ${AKONADICONTACT_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6IdentityManagementCore ${KMIME_LIB_VERSION} CONFIG REQUIRED)
find_package(KF6TextCustomEditor ${KTEXTADDONS_MIN_VERSION} CONFIG REQUIRED)

option(BUILD_DESIGNERPLUGIN "Build plugin for Qt Designer" ON)
add_feature_info(DESIGNERPLUGIN ${BUILD_DESIGNERPLUGIN} "Build plugin for Qt Designer")

ecm_setup_version(PROJECT VARIABLE_PREFIX MAILCOMMON
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/mailcommon_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPim6MailCommonConfigVersion.cmake"
                        SOVERSION 6
)


########### Targets ###########
remove_definitions( -DQT_NO_CAST_FROM_ASCII )

option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if (USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT ON)
endif()


add_definitions(-DQT_NO_CONTEXTLESS_CONNECT)
ecm_set_disabled_deprecation_versions(QT 6.6.0 KF 6.0.0)

add_subdirectory(src)
if(BUILD_TESTING)
    find_package(Qt6 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Test)
    add_subdirectory(autotests)
endif()

########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim6MailCommon")
if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KPim6MailCommon_QCH
        FILE KPim6MailCommonQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KPim6MailCommonQchTargets.cmake\")")
endif()

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KPimMailCommonConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KPim6MailCommonConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KPim6MailCommonConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KPim6MailCommonConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KPim6MailCommonTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KPim6MailCommonTargets.cmake NAMESPACE KPim6::)

install(FILES
   ${CMAKE_CURRENT_BINARY_DIR}/mailcommon_version.h
  DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/MailCommon COMPONENT Devel
)

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ki18n_install(po)

ecm_feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

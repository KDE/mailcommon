# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(PIM_VERSION "5.20.42")

project(mailcommon VERSION ${PIM_VERSION})

set(KF5_MIN_VERSION "5.95.0")

find_package(ECM ${KF5_MIN_VERSION} CONFIG REQUIRED)
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


option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

set(QT_REQUIRED_VERSION "5.15.2")

set(MAILCOMMON_LIB_VERSION ${PIM_VERSION})
set(AKONADIMIME_LIB_VERSION "5.20.41")
set(AKONADICONTACT_LIB_VERSION "5.20.40")
set(MESSAGELIB_LIB_VERSION "5.20.42")
set(KMIME_LIB_VERSION "5.20.40")
set(KMAILTRANSPORT_LIB_VERSION "5.20.40")
set(MAILIMPORTER_LIB_VERSION "5.20.40")
set(LIBKDEPIM_LIB_VERSION "5.20.40")
set(PIMCOMMON_LIB_VERSION "5.20.40")

set(PHONON_LIB_VERSION "4.11.1")
set(AKONADI_VERSION "5.20.40")

find_package(Qt${QT_MAJOR_VERSION} ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets DBus Test Xml)

find_package(KF5Archive ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Completion ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Config ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5ConfigWidgets ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5I18n ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5IconThemes ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5ItemModels ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5ItemViews ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5KIO ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5TextWidgets ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5WidgetsAddons ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5XmlGui ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5DBusAddons ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5SyntaxHighlighting ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5GuiAddons ${KF5_MIN_VERSION} CONFIG REQUIRED)

find_package(KF5Akonadi ${AKONADI_VERSION} CONFIG REQUIRED)
find_package(KF5AkonadiMime ${AKONADIMIME_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5Libkdepim ${LIBKDEPIM_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5MailImporter ${MAILIMPORTER_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5MailTransport ${KMAILTRANSPORT_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5MessageComposer ${MESSAGELIB_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5MessageList ${MESSAGELIB_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5MessageCore ${MESSAGELIB_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5MessageViewer ${MESSAGELIB_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5Mime ${KMIME_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5PimCommonAkonadi ${PIMCOMMON_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5TemplateParser ${MESSAGELIB_LIB_VERSION} CONFIG REQUIRED)
find_package(Phonon4Qt${QT_MAJOR_VERSION} ${PHONON_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5AkonadiContact ${AKONADICONTACT_LIB_VERSION} CONFIG REQUIRED)

option(BUILD_DESIGNERPLUGIN "Build plugin for Qt Designer" ON)
add_feature_info(DESIGNERPLUGIN ${BUILD_DESIGNERPLUGIN} "Build plugin for Qt Designer")

ecm_setup_version(PROJECT VARIABLE_PREFIX MAILCOMMON
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/mailcommon_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5MailCommonConfigVersion.cmake"
                        SOVERSION 5
)


########### Targets ###########
remove_definitions( -DQT_NO_CAST_FROM_ASCII )

option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if (USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT ON)
endif()


ecm_set_disabled_deprecation_versions(QT 5.15.2 KF 5.95.0)


########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5MailCommon")

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF5MailCommonConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5MailCommonConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KF5MailCommonConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5MailCommonConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KF5MailCommonTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5MailCommonTargets.cmake NAMESPACE KF5::)

install(FILES
   ${CMAKE_CURRENT_BINARY_DIR}/mailcommon_version.h
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/MailCommon COMPONENT Devel
)

add_subdirectory(src)
if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ki18n_install(po)

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF5MailCommon_QCH
        FILE KF5MailCommonQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5MailCommonQchTargets.cmake\")")
endif()

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

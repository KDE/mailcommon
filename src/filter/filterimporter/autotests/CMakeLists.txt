# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
add_akonadi_isolated_test_advanced(filterimportclawmailtest.cpp filtertestkernel.cpp "KPim${KF_MAJOR_VERSION}::MailCommon;KPim${KF_MAJOR_VERSION}::MessageComposer;KF${KF_MAJOR_VERSION}::I18n")
add_akonadi_isolated_test_advanced(filterimportthunderbirdtest.cpp filtertestkernel.cpp "KPim${KF_MAJOR_VERSION}::MailCommon;KPim${KF_MAJOR_VERSION}::MessageComposer;KF${KF_MAJOR_VERSION}::I18n")
add_akonadi_isolated_test_advanced(filterimportbalsatest.cpp filtertestkernel.cpp "KPim${KF_MAJOR_VERSION}::MailCommon;KPim${KF_MAJOR_VERSION}::MessageComposer;KF${KF_MAJOR_VERSION}::I18n")
add_akonadi_isolated_test_advanced(filterimportevolutiontest.cpp filtertestkernel.cpp "KPim${KF_MAJOR_VERSION}::MailCommon;KPim${KF_MAJOR_VERSION}::MessageComposer;KF${KF_MAJOR_VERSION}::I18n")
add_akonadi_isolated_test_advanced(filterimportprocmailtest.cpp filtertestkernel.cpp "KPim${KF_MAJOR_VERSION}::MailCommon;KPim${KF_MAJOR_VERSION}::MessageComposer;KF${KF_MAJOR_VERSION}::I18n")
add_akonadi_isolated_test_advanced(filterimportsylpheedtest.cpp filtertestkernel.cpp "KPim${KF_MAJOR_VERSION}::MailCommon;KPim${KF_MAJOR_VERSION}::MessageComposer;KF${KF_MAJOR_VERSION}::I18n")
add_akonadi_isolated_test_advanced(filterimportergmailtest.cpp filtertestkernel.cpp "KPim${KF_MAJOR_VERSION}::MailCommon;KPim${KF_MAJOR_VERSION}::MessageComposer;KF${KF_MAJOR_VERSION}::I18n")

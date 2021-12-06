# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
add_akonadi_isolated_test_advanced(filterimportclawmailtest.cpp filtertestkernel.cpp "KF5::MailCommon;KF5::MessageComposer;KF5::I18n")
add_akonadi_isolated_test_advanced(filterimportthunderbirdtest.cpp filtertestkernel.cpp "KF5::MailCommon;KF5::MessageComposer;KF5::I18n")
add_akonadi_isolated_test_advanced(filterimportbalsatest.cpp filtertestkernel.cpp "KF5::MailCommon;KF5::MessageComposer;KF5::I18n")
add_akonadi_isolated_test_advanced(filterimportevolutiontest.cpp filtertestkernel.cpp "KF5::MailCommon;KF5::MessageComposer;KF5::I18n")
add_akonadi_isolated_test_advanced(filterimportprocmailtest.cpp filtertestkernel.cpp "KF5::MailCommon;KF5::MessageComposer;KF5::I18n")
add_akonadi_isolated_test_advanced(filterimportsylpheedtest.cpp filtertestkernel.cpp "KF5::MailCommon;KF5::MessageComposer;KF5::I18n")
add_akonadi_isolated_test_advanced(filterimportergmailtest.cpp filtertestkernel.cpp "KF5::MailCommon;KF5::MessageComposer;KF5::I18n")

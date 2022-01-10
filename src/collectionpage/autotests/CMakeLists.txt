# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
macro(add_mailcommon_collectionpage_test _name)
    ecm_add_test(${ARGN}
        TEST_NAME ${_name}
        NAME_PREFIX "mailcommon-collectionpage-"
        LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Gui KF5::AkonadiCore KF5::MailCommon KF5::Completion
    )
endmacro()

add_mailcommon_collectionpage_test(expirecollectionattributetest
    expirecollectionattributetest.cpp
)

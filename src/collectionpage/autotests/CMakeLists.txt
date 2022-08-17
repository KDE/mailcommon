# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
macro(add_mailcommon_collectionpage_test _source)
    get_filename_component(_name ${_source} NAME_WE)
     ecm_add_test(${_source} ${_name}.h
        TEST_NAME ${_name}
        NAME_PREFIX "mailcommon-collectionpage-"
        LINK_LIBRARIES Qt::Test Qt::Gui KF5::AkonadiCore KF5::MailCommon KF5::Completion
    )
endmacro()

add_mailcommon_collectionpage_test(expirecollectionattributetest.cpp)

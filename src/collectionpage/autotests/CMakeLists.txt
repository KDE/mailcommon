# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
macro(add_mailcommon_collectionpage_test _source)
    get_filename_component(_name ${_source} NAME_WE)
     ecm_add_test(${_source} ${_name}.h
        TEST_NAME ${_name}
        NAME_PREFIX "mailcommon-collectionpage-"
        LINK_LIBRARIES Qt::Test Qt::Gui KPim${KF_MAJOR_VERSION}::AkonadiCore KPim${KF_MAJOR_VERSION}::MailCommon KF${KF_MAJOR_VERSION}::Completion
    )
endmacro()

add_mailcommon_collectionpage_test(expirecollectionattributetest.cpp)

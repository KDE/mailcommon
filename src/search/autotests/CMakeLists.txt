# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
macro(add_search_autotest _source)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_name}.h
        TEST_NAME ${_name}
        NAME_PREFIX "mailcommon-search-"
        LINK_LIBRARIES Qt::Test KPim6::MailCommon KF6::Codecs
    )
endmacro()

add_search_autotest(searchpatterntest.cpp)
add_search_autotest(searchruletest.cpp)
add_search_autotest(searchruleattachmenttest.cpp)
add_search_autotest(searchrulestatustest.cpp)
add_search_autotest(searchrulenumericaltest.cpp)
add_search_autotest(searchruledatetest.cpp)
add_search_autotest(searchrulestringtest.cpp)
add_search_autotest(searchruleencryptiontest.cpp)

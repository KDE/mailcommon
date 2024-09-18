# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
macro(add_search_autotest _name)
    ecm_add_test(${ARGN}
        TEST_NAME ${_name}
        NAME_PREFIX "mailcommon-search-"
        LINK_LIBRARIES Qt::Test Qt::Gui KPim6::Mime KPim6::AkonadiCore KPim6::AkonadiMime KPim6::MailCommon KF6::I18n KF6::Codecs
)
endmacro()

add_search_autotest(searchpatterntest
    searchpatterntest.cpp
    searchpatterntest.h
)
add_search_autotest(searchruletest
    searchruletest.cpp
    searchruletest.h
)

add_search_autotest(searchruleattachmenttest
	searchruleattachmenttest.cpp
	searchruleattachmenttest.h
)

add_search_autotest(searchrulestatustest
    searchrulestatustest.cpp
    searchrulestatustest.h
)
add_search_autotest(searchrulenumericaltest
    searchrulenumericaltest.cpp
    searchrulenumericaltest.h
)
add_search_autotest(searchruledatetest
    searchruledatetest.cpp
    searchruledatetest.h
)
add_search_autotest(searchrulestringtest
    searchrulestringtest.cpp
    searchrulestringtest.h
)
add_search_autotest(searchruleencryptiontest
    searchruleencryptiontest.cpp
    searchruleencryptiontest.h
)

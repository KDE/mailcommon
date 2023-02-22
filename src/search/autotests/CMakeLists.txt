# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
macro(add_search_autotest _name)
    ecm_add_test(${ARGN}
        TEST_NAME ${_name}
        NAME_PREFIX "mailcommon-search-"
        LINK_LIBRARIES Qt::Test Qt::Gui KPim${KF_MAJOR_VERSION}::Mime KF5::AkonadiCore KPim${KF_MAJOR_VERSION}::AkonadiMime KPim${KF_MAJOR_VERSION}::MailCommon KF${KF_MAJOR_VERSION}::I18n KF${KF_MAJOR_VERSION}::Codecs
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
add_search_autotest(searchrulestatustest
    searchrulestatustest.cpp
    searchrulestatustest.h
)
add_search_autotest(searchrulenumericaltest
    searchrulenumericaltest.cpp
    searchrulenumericaltest.h
    ../searchrule/searchrulenumerical.cpp
)
add_search_autotest(searchruledatetest
    searchruledatetest.cpp
    searchruledatetest.h
    ../searchrule/searchruledate.cpp
)
add_search_autotest(searchrulestringtest
    searchrulestringtest.cpp
    searchrulestringtest.h
    ../searchrule/searchrulestring.cpp
)
add_search_autotest(searchruleencryptiontest
    searchruleencryptiontest.cpp
    searchruleencryptiontest.h
    ../searchrule/searchruleencryption.cpp
)

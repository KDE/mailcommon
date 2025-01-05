# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
macro(add_snippets _source)
     get_filename_component(_name ${_source} NAME_WE)
     ecm_add_test(${_source} ${_name}.h
         TEST_NAME ${_name}
         NAME_PREFIX "mailcommon-filter-"
         LINK_LIBRARIES Qt::Test KPim6::MailCommon KF6::XmlGui
    )

endmacro()

add_snippets(snippetsmanagertest.cpp)
add_snippets(snippetwidgettest.cpp)
add_snippets(snippetattachmentwidgettest.cpp)
add_snippets(snippetselectattachmentwidgettest.cpp)
add_snippets(snippetselectattachmentdialogtest.cpp)
add_snippets(snippetcustomfileattachmentnamedialogtest.cpp)
add_snippets(snippetcustomfileattachmentnamewidgettest.cpp)

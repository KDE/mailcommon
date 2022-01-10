# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
ecm_add_test(snippetsmanagertest.cpp
    NAME_PREFIX "mailcommon-snippets-"
    LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Gui KF5::MailCommon KF5::XmlGui
)

ecm_add_test(snippetwidgettest.cpp
    NAME_PREFIX "mailcommon-snippets-"
    LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Gui KF5::MailCommon KF5::XmlGui
)

ecm_add_test(snippetattachmentwidgettest.cpp
    NAME_PREFIX "mailcommon-snippets-"
    LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Gui KF5::MailCommon
)

ecm_add_test(snippetselectattachmentwidgettest.cpp
    NAME_PREFIX "mailcommon-snippets-"
    LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Gui KF5::MailCommon
)

ecm_add_test(snippetselectattachmentdialogtest.cpp
    NAME_PREFIX "mailcommon-snippets-"
    LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Gui KF5::MailCommon
)

ecm_add_test(snippetcustomfileattachmentnamedialogtest.cpp
    NAME_PREFIX "mailcommon-snippets-"
    LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Gui KF5::MailCommon
)

ecm_add_test(snippetcustomfileattachmentnamewidgettest.cpp
    NAME_PREFIX "mailcommon-snippets-"
    LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Gui KF5::MailCommon
)

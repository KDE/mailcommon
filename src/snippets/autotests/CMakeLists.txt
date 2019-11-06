ecm_add_test(snippetsmanagertest.cpp
    NAME_PREFIX "mailcommon-snippets-"
    LINK_LIBRARIES Qt5::Test Qt5::Gui KF5::MailCommon KF5::XmlGui
)

ecm_add_test(snippetwidgettest.cpp
    NAME_PREFIX "mailcommon-snippets-"
    LINK_LIBRARIES Qt5::Test Qt5::Gui KF5::MailCommon KF5::XmlGui
)

ecm_add_test(snippetattachmentwidgettest.cpp
    NAME_PREFIX "mailcommon-snippets-"
    LINK_LIBRARIES Qt5::Test Qt5::Gui KF5::MailCommon
)

ecm_add_test(snippetselectattachmentwidgettest.cpp
    NAME_PREFIX "mailcommon-snippets-"
    LINK_LIBRARIES Qt5::Test Qt5::Gui KF5::MailCommon
)

ecm_add_test(snippetselectattachmentdialogtest.cpp
    NAME_PREFIX "mailcommon-snippets-"
    LINK_LIBRARIES Qt5::Test Qt5::Gui KF5::MailCommon
)

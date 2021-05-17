ecm_add_test(mdnstateattributetest.cpp
    TEST_NAME mdnattributetest
    NAME_PREFIX "mailcommon-mdn-"
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::AkonadiCore KF5::MailCommon
)

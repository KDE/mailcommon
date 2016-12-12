macro(add_mailcommon_plugincheckbeforeclosing_test _name)
    ecm_add_test(${ARGN}
        TEST_NAME ${_name}
        NAME_PREFIX "mailcommon-plugincheckbeforeclosing-"
        LINK_LIBRARIES Qt5::Test Qt5::Gui KF5::MailCommon
    )
endmacro()

add_mailcommon_plugincheckbeforeclosing_test(plugincheckbeforeclosingtest plugincheckbeforeclosingtest.cpp)

add_mailcommon_plugincheckbeforeclosing_test(plugincheckbeforeclosinginterfacetest plugincheckbeforeclosinginterfacetest.cpp)

if (USE_NEW_DESIGNERPLUGIN_MACRO)
    include(ECMAddQtDesignerPlugin)
    ecm_qtdesignerplugin_widget(MailCommon::SearchPatternEdit
        INCLUDE_FILE "mailcommon/searchpatternedit.h"
        TOOLTIP "Search pattern editor (KDE-PIM)"
        WHATSTHIS "This widget is intended to be used in the filter configuration as well as in the message search dialogs."
        GROUP "Input (KDE-PIM)"
        )

    ecm_add_qtdesignerplugin(mailcommonwidgets
        NAME KDEPimMailCommonWidgets
        OUTPUT_NAME mailcommonwidgets
        WIDGETS
        MailCommon::SearchPatternEdit
        LINK_LIBRARIES
        KF5::MailCommon
        KF5::Libkdepim
        KF5::I18n
        INSTALL_DESTINATION "${KDE_INSTALL_QTPLUGINDIR}/designer"
        COMPONENT Devel
        )
else()
    if (KF5DesignerPlugin_FOUND)
        kf5designerplugin_add_plugin(mailcommonwidgets mailcommon.widgets)

        target_link_libraries(mailcommonwidgets
            KF5::MailCommon
            KF5::Libkdepim
            KF5::I18n
            )
        if(NOT WIN32)
            # Since there are no libraries provided by this module,
            # there is no point including the build tree in RPath,
            # and then having to edit it at install time.
            set_target_properties(mailcommonwidgets PROPERTIES
                SKIP_BUILD_RPATH TRUE
                BUILD_WITH_INSTALL_RPATH TRUE
                )
        endif()

        install(TARGETS
            mailcommonwidgets
            DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/designer
            )
    endif()
endif()

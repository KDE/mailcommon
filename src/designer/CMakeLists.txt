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

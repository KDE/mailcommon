# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
include(ECMAddQtDesignerPlugin)
ecm_qtdesignerplugin_widget(MailCommon::SearchPatternEdit
    INCLUDE_FILE "MailCommon/SearchPatternEdit"
    TOOLTIP "Search pattern editor (KDE-PIM)"
    WHATSTHIS "This widget is intended to be used in the filter configuration as well as in the message search dialogs."
    GROUP "Input (KDE-PIM)"
    )

ecm_add_qtdesignerplugin(mailcommonwidgets
    NAME KDEPimMailCommonWidgets
    OUTPUT_NAME mailcommon${QT_MAJOR_VERSION}widgets
    WIDGETS
    MailCommon::SearchPatternEdit
    LINK_LIBRARIES
    KPim6::MailCommon
    KPim6::Libkdepim
    KF6::I18n
    INSTALL_DESTINATION "${KDE_INSTALL_QTPLUGINDIR}/designer"
    COMPONENT Devel
    )

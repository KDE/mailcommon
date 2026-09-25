# SPDX-FileCopyrightText: 2026 Carl Schwan <carl@carlschwan.eu>
# SPDX-License-Identifier: BSD-3-Clause

ecm_add_qml_module(mailcommon_quick_plugin
    URI org.kde.mailcommon
    GENERATE_PLUGIN_SOURCE
    DEPENDENCIES
        org.kde.akonadi
)

target_sources(
    mailcommon_quick_plugin
    PRIVATE
        types.cpp
        types.h
)

target_link_libraries(
    mailcommon_quick_plugin
    PRIVATE
        KPim6::MailCommon
        Qt::Qml
)

ecm_finalize_qml_module(mailcommon_quick_plugin
    DESTINATION ${KDE_INSTALL_QMLDIR}
    BUILD_SHARED_LIBS OFF
)

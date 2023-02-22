# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

add_akonadi_isolated_test(foldertreewidgettest.cpp)
target_link_libraries(foldertreewidgettest KPim${KF_MAJOR_VERSION}::Mime KF5::AkonadiWidgets KPim${KF_MAJOR_VERSION}::MailCommon)

add_akonadi_isolated_test(favoritestest.cpp)
target_link_libraries(favoritestest KPim${KF_MAJOR_VERSION}::Mime KF5::AkonadiWidgets KPim${KF_MAJOR_VERSION}::MailCommon)

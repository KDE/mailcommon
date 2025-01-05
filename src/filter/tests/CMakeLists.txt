# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
add_executable(filterimporterexportergui filterimporterexportergui.cpp filterimporterexportergui.h)
ecm_mark_as_test(filterimporterexportergui)
target_link_libraries(filterimporterexportergui
  Qt::Core
  Qt::Gui
  KPim6::MailCommon
  KF6::I18n
)

add_executable(invalidfilterdialoggui invalidfilterdialoggui.cpp)
ecm_mark_as_test(invalidfilterdialoggui)
target_link_libraries(invalidfilterdialoggui
  Qt::Core
  Qt::Gui
  KPim6::MailCommon
  KF6::I18n
)

add_executable(kmfilterdialogtestgui kmfilterdialogtestgui.cpp ../../../autotests/dummykernel.cpp ../../../autotests/dummykernel.h kmfilterdialogtestgui.h)
ecm_mark_as_test(kmfilterdialogtestgui)
target_link_libraries(kmfilterdialogtestgui
  Qt::Core
  Qt::Gui
  KPim6::MailCommon
  KF6::I18n
)

add_executable(filteractionmissingsoundurldialogui filteractionmissingsoundurldialogui.cpp ../../../autotests/dummykernel.cpp ../../../autotests/dummykernel.h)
ecm_mark_as_test(filteractionmissingsoundurldialogui)
target_link_libraries(filteractionmissingsoundurldialogui
  Qt::Core
  Qt::Gui
  KPim6::MailCommon
  KF6::I18n
  KF6::KIOWidgets
)

add_executable(filteractionmissingtemplatedialoggui filteractionmissingtemplatedialoggui.cpp ../../../autotests/dummykernel.cpp ../../../autotests/dummykernel.h)
ecm_mark_as_test(filteractionmissingtemplatedialoggui)
target_link_libraries(filteractionmissingtemplatedialoggui
  Qt::Core
  Qt::Gui
  KPim6::MailCommon
  KF6::I18n
  KF6::Completion
)

add_executable(filteractionmissingtransportdialoggui filteractionmissingtransportdialoggui.cpp ../../../autotests/dummykernel.cpp ../../../autotests/dummykernel.h)
ecm_mark_as_test(filteractionmissingtransportdialoggui)
target_link_libraries(filteractionmissingtransportdialoggui
  Qt::Core
  Qt::Gui
  KPim6::MailCommon
  KF6::I18n
  KF6::Completion
  KPim6::MailTransport
)

add_executable(filteractionmissingaccountdialoggui filteractionmissingaccountdialoggui.cpp ../../../autotests/dummykernel.cpp ../../../autotests/dummykernel.h )
ecm_mark_as_test(filteractionmissingaccountdialoggui)
target_link_libraries(filteractionmissingaccountdialoggui
  Qt::Core
  Qt::Gui
  KPim6::MailCommon
  KF6::I18n
  KF6::Completion
)

add_executable(filteractionmissingtagdialoggui filteractionmissingtagdialoggui.cpp ../../../autotests/dummykernel.cpp ../../../autotests/dummykernel.h)
ecm_mark_as_test(filteractionmissingtagdialoggui)
target_link_libraries(filteractionmissingtagdialoggui
  Qt::Core
  Qt::Gui
  KPim6::MailCommon
  KF6::I18n
  KF6::Completion
)

add_executable(filteractionmissingidentitydialoggui filteractionmissingidentitydialoggui.cpp ../../../autotests/dummykernel.cpp ../../../autotests/dummykernel.h)
ecm_mark_as_test(filteractionmissingidentitydialoggui)
target_link_libraries(filteractionmissingidentitydialoggui
  Qt::Core
  Qt::Gui
  KPim6::MailCommon
  KF6::I18n
  KF6::Completion
)

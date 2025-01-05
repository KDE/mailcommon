# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
add_executable(filterimporterexportergui filterimporterexportergui.cpp filterimporterexportergui.h)
ecm_mark_as_test(filterimporterexportergui)
target_link_libraries(filterimporterexportergui
  KPim6::MailCommon
)

add_executable(invalidfilterdialoggui invalidfilterdialoggui.cpp)
ecm_mark_as_test(invalidfilterdialoggui)
target_link_libraries(invalidfilterdialoggui
  KPim6::MailCommon
)

add_executable(kmfilterdialogtestgui kmfilterdialogtestgui.cpp ../../../autotests/dummykernel.cpp ../../../autotests/dummykernel.h kmfilterdialogtestgui.h)
ecm_mark_as_test(kmfilterdialogtestgui)
target_link_libraries(kmfilterdialogtestgui
  KPim6::MailCommon
)

add_executable(filteractionmissingsoundurldialogui filteractionmissingsoundurldialogui.cpp ../../../autotests/dummykernel.cpp ../../../autotests/dummykernel.h)
ecm_mark_as_test(filteractionmissingsoundurldialogui)
target_link_libraries(filteractionmissingsoundurldialogui
  Qt::Gui
  KPim6::MailCommon
  KF6::KIOWidgets
)

add_executable(filteractionmissingtemplatedialoggui filteractionmissingtemplatedialoggui.cpp ../../../autotests/dummykernel.cpp ../../../autotests/dummykernel.h)
ecm_mark_as_test(filteractionmissingtemplatedialoggui)
target_link_libraries(filteractionmissingtemplatedialoggui
  KPim6::MailCommon

)

add_executable(filteractionmissingtransportdialoggui filteractionmissingtransportdialoggui.cpp ../../../autotests/dummykernel.cpp ../../../autotests/dummykernel.h)
ecm_mark_as_test(filteractionmissingtransportdialoggui)
target_link_libraries(filteractionmissingtransportdialoggui
  KPim6::MailCommon

  KPim6::MailTransport
)

add_executable(filteractionmissingaccountdialoggui filteractionmissingaccountdialoggui.cpp ../../../autotests/dummykernel.cpp ../../../autotests/dummykernel.h )
ecm_mark_as_test(filteractionmissingaccountdialoggui)
target_link_libraries(filteractionmissingaccountdialoggui
  KPim6::MailCommon

)

add_executable(filteractionmissingtagdialoggui filteractionmissingtagdialoggui.cpp ../../../autotests/dummykernel.cpp ../../../autotests/dummykernel.h)
ecm_mark_as_test(filteractionmissingtagdialoggui)
target_link_libraries(filteractionmissingtagdialoggui
  KPim6::MailCommon

)

add_executable(filteractionmissingidentitydialoggui filteractionmissingidentitydialoggui.cpp ../../../autotests/dummykernel.cpp ../../../autotests/dummykernel.h)
ecm_mark_as_test(filteractionmissingidentitydialoggui)
target_link_libraries(filteractionmissingidentitydialoggui
  KPim6::MailCommon

)

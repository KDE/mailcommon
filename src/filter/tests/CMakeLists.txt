
set(filterimporterexportergui_SRCS filterimporterexportergui.cpp)
add_executable(filterimporterexportergui ${filterimporterexportergui_SRCS})
ecm_mark_as_test(filterimporterexportergui)
target_link_libraries(filterimporterexportergui
  Qt5::Core
  Qt5::Gui
  KF5::MailCommon
  KF5::I18n
)

set(invalidfilterdialoggui_SRCS invalidfilterdialoggui.cpp)
add_executable(invalidfilterdialoggui ${invalidfilterdialoggui_SRCS})
ecm_mark_as_test(invalidfilterdialoggui)
target_link_libraries(invalidfilterdialoggui
  Qt5::Core
  Qt5::Gui
  KF5::MailCommon
  KF5::I18n
)

set(mndadvicedialoggui_SRCS mndadvicedialoggui.cpp)
add_executable(mndadvicedialoggui ${mndadvicedialoggui_SRCS})
ecm_mark_as_test(mndadvicedialoggui)
target_link_libraries(mndadvicedialoggui
  Qt5::Core
  Qt5::Gui
  KF5::MailCommon
  KF5::I18n
)


set(kmfilterdialogtestgui_SRCS kmfilterdialogtestgui.cpp ../../../autotests/dummykernel.cpp)
add_executable(kmfilterdialogtestgui ${kmfilterdialogtestgui_SRCS})
ecm_mark_as_test(kmfilterdialogtestgui)
target_link_libraries(kmfilterdialogtestgui
  Qt5::Core
  Qt5::Gui
  KF5::MailCommon
  KF5::I18n
)

set(filteractionmissingsoundurldialogui_SRCS filteractionmissingsoundurldialogui.cpp ../../../autotests/dummykernel.cpp ../dialog/filteractionmissingsoundurldialog.cpp)
add_executable(filteractionmissingsoundurldialogui ${filteractionmissingsoundurldialogui_SRCS})
ecm_mark_as_test(filteractionmissingsoundurldialogui)
target_link_libraries(filteractionmissingsoundurldialogui
  Qt5::Core
  Qt5::Gui
  KF5::MailCommon
  KF5::I18n
  KF5::KIOWidgets
)

set(filteractionmissingtemplatedialoggui_SRCS filteractionmissingtemplatedialoggui.cpp ../../../autotests/dummykernel.cpp ../dialog/filteractionmissingtemplatedialog.cpp)
add_executable(filteractionmissingtemplatedialoggui ${filteractionmissingtemplatedialoggui_SRCS})
ecm_mark_as_test(filteractionmissingtemplatedialoggui)
target_link_libraries(filteractionmissingtemplatedialoggui
  Qt5::Core
  Qt5::Gui
  KF5::MailCommon
  KF5::I18n
  KF5::Completion
)

set(filteractionmissingtransportdialoggui_SRCS filteractionmissingtransportdialoggui.cpp ../../../autotests/dummykernel.cpp ../dialog/filteractionmissingtransportdialog.cpp)
add_executable(filteractionmissingtransportdialoggui ${filteractionmissingtransportdialoggui_SRCS})
ecm_mark_as_test(filteractionmissingtransportdialoggui)
target_link_libraries(filteractionmissingtransportdialoggui
  Qt5::Core
  Qt5::Gui
  KF5::MailCommon
  KF5::I18n
  KF5::Completion
  KF5::MailTransport
)

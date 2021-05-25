
add_executable(filterimporterexportergui filterimporterexportergui.cpp)
ecm_mark_as_test(filterimporterexportergui)
target_link_libraries(filterimporterexportergui
  Qt::Core
  Qt::Gui
  KF5::MailCommon
  KF5::I18n
)

add_executable(invalidfilterdialoggui invalidfilterdialoggui.cpp)
ecm_mark_as_test(invalidfilterdialoggui)
target_link_libraries(invalidfilterdialoggui
  Qt::Core
  Qt::Gui
  KF5::MailCommon
  KF5::I18n
)

add_executable(mndadvicedialoggui mndadvicedialoggui.cpp)
ecm_mark_as_test(mndadvicedialoggui)
target_link_libraries(mndadvicedialoggui
  Qt::Core
  Qt::Gui
  KF5::MailCommon
  KF5::I18n
)


add_executable(kmfilterdialogtestgui kmfilterdialogtestgui.cpp ../../../autotests/dummykernel.cpp)
ecm_mark_as_test(kmfilterdialogtestgui)
target_link_libraries(kmfilterdialogtestgui
  Qt::Core
  Qt::Gui
  KF5::MailCommon
  KF5::I18n
)

add_executable(filteractionmissingsoundurldialogui filteractionmissingsoundurldialogui.cpp ../../../autotests/dummykernel.cpp ../dialog/filteractionmissingsoundurldialog.cpp)
ecm_mark_as_test(filteractionmissingsoundurldialogui)
target_link_libraries(filteractionmissingsoundurldialogui
  Qt::Core
  Qt::Gui
  KF5::MailCommon
  KF5::I18n
  KF5::KIOWidgets
)

add_executable(filteractionmissingtemplatedialoggui filteractionmissingtemplatedialoggui.cpp ../../../autotests/dummykernel.cpp ../dialog/filteractionmissingtemplatedialog.cpp)
ecm_mark_as_test(filteractionmissingtemplatedialoggui)
target_link_libraries(filteractionmissingtemplatedialoggui
  Qt::Core
  Qt::Gui
  KF5::MailCommon
  KF5::I18n
  KF5::Completion
)

add_executable(filteractionmissingtransportdialoggui filteractionmissingtransportdialoggui.cpp ../../../autotests/dummykernel.cpp ../dialog/filteractionmissingtransportdialog.cpp)
ecm_mark_as_test(filteractionmissingtransportdialoggui)
target_link_libraries(filteractionmissingtransportdialoggui
  Qt::Core
  Qt::Gui
  KF5::MailCommon
  KF5::I18n
  KF5::Completion
  KF5::MailTransport
)

add_executable(filteractionmissingaccountdialoggui filteractionmissingaccountdialoggui.cpp ../../../autotests/dummykernel.cpp  ../dialog/filteractionmissingaccountdialog.cpp)
ecm_mark_as_test(filteractionmissingaccountdialoggui)
target_link_libraries(filteractionmissingaccountdialoggui
  Qt::Core
  Qt::Gui
  KF5::MailCommon
  KF5::I18n
  KF5::Completion
)

add_executable(filteractionmissingtagdialoggui filteractionmissingtagdialoggui.cpp ../../../autotests/dummykernel.cpp ../dialog/filteractionmissingtagdialog.cpp)
ecm_mark_as_test(filteractionmissingtagdialoggui)
target_link_libraries(filteractionmissingtagdialoggui
  Qt::Core
  Qt::Gui
  KF5::MailCommon
  KF5::I18n
  KF5::Completion
)

add_executable(filteractionmissingidentitydialoggui filteractionmissingidentitydialoggui.cpp ../../../autotests/dummykernel.cpp ../dialog/filteractionmissingidentitydialog.cpp)
ecm_mark_as_test(filteractionmissingidentitydialoggui)
target_link_libraries(filteractionmissingidentitydialoggui
  Qt::Core
  Qt::Gui
  KF5::MailCommon
  KF5::I18n
  KF5::Completion
)

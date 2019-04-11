if (KDEPIM_RUN_AKONADI_TEST)
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
	set(KDEPIMLIBS_RUN_ISOLATED_TESTS TRUE)
	set(KDEPIMLIBS_RUN_SQLITE_ISOLATED_TESTS TRUE)

	add_akonadi_isolated_test(foldertreewidgettest.cpp)
	target_link_libraries(foldertreewidgettest KF5::Mime KF5::AkonadiWidgets KF5::MailCommon)

	add_akonadi_isolated_test(favoritestest.cpp)
	target_link_libraries(favoritestest KF5::Mime KF5::AkonadiWidgets KF5::MailCommon)
endif()	


qt_add_executable(SageMessageBoxDlgTest SageMessageBoxDlgTest.cpp)
qt_add_resources(SageMessageBoxDlgTest "sage_test_fonts"
    PREFIX "/fonts"
    BASE ${SAGE_FONT_RESOURCE_DIR}
    FILES ${SAGE_FONT_RESOURCES}
)
target_link_libraries(SageMessageBoxDlgTest PRIVATE sage_define sage_ui Qt::Test)
add_test(NAME SageMessageBoxDlgTest COMMAND SageMessageBoxDlgTest)
set_tests_properties(SageMessageBoxDlgTest PROPERTIES ENVIRONMENT QT_QPA_PLATFORM=offscreen)

qt_add_executable(SageLoginDlgTest SageLoginDlgTest.cpp ../SageTestAuth.h ../SageTestModalDriver.h)
qt_add_resources(SageLoginDlgTest "sage_test_fonts"
    PREFIX "/fonts"
    BASE ${SAGE_FONT_RESOURCE_DIR}
    FILES ${SAGE_FONT_RESOURCES}
)
target_include_directories(SageLoginDlgTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(SageLoginDlgTest PRIVATE sage_ui Qt::Test)
add_test(NAME SageLoginDlgTest COMMAND SageLoginDlgTest)
set_tests_properties(SageLoginDlgTest PROPERTIES ENVIRONMENT QT_QPA_PLATFORM=offscreen)

qt_add_executable(SagePasswordChangeDlgTest SagePasswordChangeDlgTest.cpp ../SageTestAuth.h ../SageTestModalDriver.h)
qt_add_resources(SagePasswordChangeDlgTest "sage_test_fonts"
    PREFIX "/fonts"
    BASE ${SAGE_FONT_RESOURCE_DIR}
    FILES ${SAGE_FONT_RESOURCES}
)
target_include_directories(SagePasswordChangeDlgTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(SagePasswordChangeDlgTest PRIVATE sage_ui Qt::Test)
add_test(NAME SagePasswordChangeDlgTest COMMAND SagePasswordChangeDlgTest)
set_tests_properties(SagePasswordChangeDlgTest PROPERTIES ENVIRONMENT QT_QPA_PLATFORM=offscreen)

qt_add_executable(SageMessageBoxDlgTest SageMessageBoxDlgTest.cpp)
qt_add_resources(SageMessageBoxDlgTest "sage_test_fonts"
    PREFIX "/fonts"
    BASE ${SAGE_FONT_RESOURCE_DIR}
    FILES ${SAGE_FONT_RESOURCES}
)
target_link_libraries(SageMessageBoxDlgTest PRIVATE sage_define sage_ui Qt::Test)
add_test(NAME SageMessageBoxDlgTest COMMAND SageMessageBoxDlgTest)
set_tests_properties(SageMessageBoxDlgTest PROPERTIES ENVIRONMENT QT_QPA_PLATFORM=offscreen)

qt_add_executable(SageMainWindowTest SageMainWindowTest.cpp)
qt_add_resources(SageMainWindowTest "sage_test_fonts"
    PREFIX "/fonts"
    BASE ${SAGE_FONT_RESOURCE_DIR}
    FILES ${SAGE_FONT_RESOURCES}
)
target_link_libraries(SageMainWindowTest PRIVATE sage_ui Qt::Test)
add_test(NAME SageMainWindowTest COMMAND SageMainWindowTest)
set_tests_properties(SageMainWindowTest PROPERTIES ENVIRONMENT QT_QPA_PLATFORM=offscreen)

qt_add_executable(SageStatusCardTest SageStatusCardTest.cpp)
qt_add_resources(SageStatusCardTest "sage_test_fonts"
    PREFIX "/fonts"
    BASE ${SAGE_FONT_RESOURCE_DIR}
    FILES ${SAGE_FONT_RESOURCES}
)
target_link_libraries(SageStatusCardTest PRIVATE sage_ui Qt::Test)
add_test(NAME SageStatusCardTest COMMAND SageStatusCardTest)
set_tests_properties(SageStatusCardTest PROPERTIES ENVIRONMENT QT_QPA_PLATFORM=offscreen)

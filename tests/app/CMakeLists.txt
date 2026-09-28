qt_add_executable(SageLoginFlowTest SageLoginFlowTest.cpp)
qt_add_resources(SageLoginFlowTest "sage_test_fonts"
    PREFIX "/fonts"
    BASE ${SAGE_FONT_RESOURCE_DIR}
    FILES ${SAGE_FONT_RESOURCES}
)
target_link_libraries(SageLoginFlowTest PRIVATE sage_infra sage_ui Qt::Test)
add_test(NAME SageLoginFlowTest COMMAND SageLoginFlowTest)
set_tests_properties(SageLoginFlowTest PROPERTIES ENVIRONMENT QT_QPA_PLATFORM=offscreen)

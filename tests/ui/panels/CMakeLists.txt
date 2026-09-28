qt_add_executable(SageSidebarPanelTest SageSidebarPanelTest.cpp SageTestWorkflowHandler.h)
qt_add_resources(SageSidebarPanelTest "sage_test_fonts"
    PREFIX "/fonts"
    BASE ${SAGE_FONT_RESOURCE_DIR}
    FILES ${SAGE_FONT_RESOURCES}
)
target_link_libraries(SageSidebarPanelTest PRIVATE sage_ui Qt::Test)
add_test(NAME SageSidebarPanelTest COMMAND SageSidebarPanelTest)
set_tests_properties(SageSidebarPanelTest PROPERTIES ENVIRONMENT QT_QPA_PLATFORM=offscreen)

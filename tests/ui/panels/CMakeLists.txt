set(SAGE_TEST_WORKFLOW_DIR ${CMAKE_SOURCE_DIR}/tests/core/workflow)

qt_add_executable(SageSidebarPanelTest SageSidebarPanelTest.cpp ${SAGE_TEST_WORKFLOW_DIR}/SageTestWorkflowHandler.h)
qt_add_resources(SageSidebarPanelTest "sage_test_fonts"
    PREFIX "/fonts"
    BASE ${SAGE_FONT_RESOURCE_DIR}
    FILES ${SAGE_FONT_RESOURCES}
)
target_include_directories(SageSidebarPanelTest PRIVATE ${SAGE_TEST_WORKFLOW_DIR})
target_link_libraries(SageSidebarPanelTest PRIVATE sage_ui Qt::Test)
add_test(NAME SageSidebarPanelTest COMMAND SageSidebarPanelTest)
set_tests_properties(SageSidebarPanelTest PROPERTIES ENVIRONMENT QT_QPA_PLATFORM=offscreen)

qt_add_executable(SageHeaderPanelTest SageHeaderPanelTest.cpp)
qt_add_resources(SageHeaderPanelTest "sage_test_fonts"
    PREFIX "/fonts"
    BASE ${SAGE_FONT_RESOURCE_DIR}
    FILES ${SAGE_FONT_RESOURCES}
)
target_link_libraries(SageHeaderPanelTest PRIVATE sage_ui Qt::Test)
add_test(NAME SageHeaderPanelTest COMMAND SageHeaderPanelTest)
set_tests_properties(SageHeaderPanelTest PROPERTIES ENVIRONMENT QT_QPA_PLATFORM=offscreen)

qt_add_executable(SageWorkspacePanelTest SageWorkspacePanelTest.cpp ${SAGE_TEST_WORKFLOW_DIR}/SageTestWorkflowHandler.h
    ../SageTestModalDriver.h)
qt_add_resources(SageWorkspacePanelTest "sage_test_fonts"
    PREFIX "/fonts"
    BASE ${SAGE_FONT_RESOURCE_DIR}
    FILES ${SAGE_FONT_RESOURCES}
)
target_include_directories(SageWorkspacePanelTest PRIVATE ${SAGE_TEST_WORKFLOW_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(SageWorkspacePanelTest PRIVATE sage_ui Qt::Test)
add_test(NAME SageWorkspacePanelTest COMMAND SageWorkspacePanelTest)
set_tests_properties(SageWorkspacePanelTest PROPERTIES ENVIRONMENT QT_QPA_PLATFORM=offscreen)

qt_add_executable(SageWorkflowInputPanelTest SageWorkflowInputPanelTest.cpp ${SAGE_TEST_WORKFLOW_DIR}/SageTestWorkflowHandler.h)
qt_add_resources(SageWorkflowInputPanelTest "sage_test_fonts"
    PREFIX "/fonts"
    BASE ${SAGE_FONT_RESOURCE_DIR}
    FILES ${SAGE_FONT_RESOURCES}
)
target_include_directories(SageWorkflowInputPanelTest PRIVATE ${SAGE_TEST_WORKFLOW_DIR})
target_link_libraries(SageWorkflowInputPanelTest PRIVATE sage_ui Qt::Test)
add_test(NAME SageWorkflowInputPanelTest COMMAND SageWorkflowInputPanelTest)
set_tests_properties(SageWorkflowInputPanelTest PROPERTIES ENVIRONMENT QT_QPA_PLATFORM=offscreen)

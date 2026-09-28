qt_add_executable(SageMainWindowTest SageMainWindowTest.cpp ../SageTestAuth.h)
qt_add_resources(SageMainWindowTest "sage_test_fonts"
    PREFIX "/fonts"
    BASE ${SAGE_FONT_RESOURCE_DIR}
    FILES ${SAGE_FONT_RESOURCES}
)
target_include_directories(SageMainWindowTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(SageMainWindowTest PRIVATE sage_ui Qt::Test)
add_test(NAME SageMainWindowTest COMMAND SageMainWindowTest)
set_tests_properties(SageMainWindowTest PROPERTIES ENVIRONMENT QT_QPA_PLATFORM=offscreen)

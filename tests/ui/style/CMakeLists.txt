qt_add_executable(SageFontRegistryTest SageFontRegistryTest.cpp)
qt_add_resources(SageFontRegistryTest "sage_test_fonts"
    PREFIX "/fonts"
    BASE ${SAGE_FONT_RESOURCE_DIR}
    FILES ${SAGE_FONT_RESOURCES}
)
target_link_libraries(SageFontRegistryTest PRIVATE sage_ui Qt::Test)
add_test(NAME SageFontRegistryTest COMMAND SageFontRegistryTest)
set_tests_properties(SageFontRegistryTest PROPERTIES ENVIRONMENT QT_QPA_PLATFORM=offscreen)

qt_add_executable(SageStyleTest SageStyleTest.cpp)
target_link_libraries(SageStyleTest PRIVATE sage_ui Qt::Test)
add_test(NAME SageStyleTest COMMAND SageStyleTest)
set_tests_properties(SageStyleTest PROPERTIES ENVIRONMENT QT_QPA_PLATFORM=offscreen)

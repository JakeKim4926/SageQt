qt_add_executable(SageDbConfigTest SageDbConfigTest.cpp)
target_link_libraries(SageDbConfigTest PRIVATE sage_infra Qt::Test)
add_test(NAME SageDbConfigTest COMMAND SageDbConfigTest)

qt_add_executable(SageDbConnectionTest SageDbConnectionTest.cpp)
target_link_libraries(SageDbConnectionTest PRIVATE sage_infra Qt::Concurrent Qt::Sql Qt::Test)
add_test(NAME SageDbConnectionTest COMMAND SageDbConnectionTest)

qt_add_executable(SageSchemaInitializerTest SageSchemaInitializerTest.cpp)
target_link_libraries(SageSchemaInitializerTest PRIVATE sage_infra Qt::Sql Qt::Test)
add_test(NAME SageSchemaInitializerTest COMMAND SageSchemaInitializerTest)

qt_add_executable(SageUserRepositoryTest SageUserRepositoryTest.cpp)
target_link_libraries(SageUserRepositoryTest PRIVATE sage_infra Qt::Sql Qt::Test)
add_test(NAME SageUserRepositoryTest COMMAND SageUserRepositoryTest)

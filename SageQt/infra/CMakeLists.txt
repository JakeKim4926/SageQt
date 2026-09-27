qt_add_library(sage_infra STATIC
    infra/db/SageDbConfig.cpp
    infra/db/SageDbConfig.h
    infra/db/SageDbConnection.cpp
    infra/db/SageDbConnection.h
    infra/db/SageDbDefine.h
    infra/db/SageSchemaInitializer.cpp
    infra/db/SageSchemaInitializer.h
)

target_include_directories(sage_infra
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(sage_infra
    PUBLIC
        Qt::Core
        sage_core
        sage_define
    PRIVATE
        Qt::Sql
)

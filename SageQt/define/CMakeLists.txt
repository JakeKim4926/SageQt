configure_file(SageAppInfo.h.in ${CMAKE_CURRENT_BINARY_DIR}/SageAppInfo.h @ONLY)

add_library(sage_define INTERFACE
    SageDefine.h
)

target_include_directories(sage_define
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(sage_define
    INTERFACE
        Qt::Core
)

find_package(Qt6 6.11 REQUIRED COMPONENTS Core Network Sql Widgets)
qt_standard_project_setup(REQUIRES 6.11)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wextra>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wpedantic>
)

add_subdirectory(define)
add_subdirectory(core)
add_subdirectory(infra)
add_subdirectory(ui)

qt_add_executable(SageQt
    main.cpp
)

set_target_properties(SageQt
    PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
)

target_link_libraries(SageQt
    PRIVATE
        sage_core
        sage_define
        sage_infra
        sage_ui
        Qt::Widgets
)

add_subdirectory(${PROJECT_SOURCE_DIR}/tests ${PROJECT_BINARY_DIR}/tests)

option(SAGE_BUILD_TOOLS "Build the developer tools under tools/" OFF)
if(SAGE_BUILD_TOOLS)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tools/font-probe ${PROJECT_BINARY_DIR}/tools/font-probe)
endif()

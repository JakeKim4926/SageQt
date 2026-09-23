find_package(Qt6 6.11 REQUIRED COMPONENTS Core Widgets)
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
        sage_ui
        Qt::Widgets
)

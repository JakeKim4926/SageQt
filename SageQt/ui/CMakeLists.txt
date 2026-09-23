qt_add_library(sage_ui STATIC
    ui/window/SageMainWindow.cpp
    ui/window/SageMainWindow.h
)

target_include_directories(sage_ui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(sage_ui
    PUBLIC
        Qt::Widgets
    PRIVATE
        sage_define
)

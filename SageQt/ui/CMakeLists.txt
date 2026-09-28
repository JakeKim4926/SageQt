qt_add_library(sage_ui STATIC
    ui/dialogs/SageFramelessDlg.cpp
    ui/dialogs/SageFramelessDlg.h
    ui/dialogs/SageMessageBoxDlg.cpp
    ui/dialogs/SageMessageBoxDlg.h
    ui/style/SageDesignDefine.h
    ui/style/SageFontCatalog.cpp
    ui/style/SageFontCatalog.h
    ui/style/SageFontRegistry.cpp
    ui/style/SageFontRegistry.h
    ui/style/SageIconEngine.cpp
    ui/style/SageIconEngine.h
    ui/style/SageStyle.cpp
    ui/style/SageStyle.h
    ui/style/SageStyleDefine.h
    ui/widgets/SageButton.cpp
    ui/widgets/SageButton.h
    ui/widgets/SageDialogCaptionBar.cpp
    ui/widgets/SageDialogCaptionBar.h
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

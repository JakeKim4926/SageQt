find_package(Qt6 6.11 REQUIRED COMPONENTS Concurrent Core Network Sql Widgets)
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

set(SAGE_FONT_RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
set(SAGE_FONT_RESOURCES
    ${SAGE_FONT_RESOURCE_DIR}/GmarketSansTTFBold.ttf
    ${SAGE_FONT_RESOURCE_DIR}/PretendardBold.ttf
    ${SAGE_FONT_RESOURCE_DIR}/PretendardRegular.ttf
    ${SAGE_FONT_RESOURCE_DIR}/PretendardSemiBold.ttf
)

qt_add_resources(SageQt "sage_fonts"
    PREFIX "/fonts"
    BASE ${SAGE_FONT_RESOURCE_DIR}
    FILES ${SAGE_FONT_RESOURCES}
)

set(SAGE_APP_ICON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons)
set(SAGE_APP_ICON_NAME "sageqt")
set(SAGE_APP_ICON_ICO ${SAGE_APP_ICON_DIR}/sageqt.ico)
set(SAGE_APP_ICON_ICNS ${SAGE_APP_ICON_DIR}/sageqt.icns)
set(SAGE_APP_ICON_PNG ${SAGE_APP_ICON_DIR}/sageqt-256.png)

qt_add_resources(SageQt "sage_icons"
    PREFIX "/icons"
    BASE ${SAGE_APP_ICON_DIR}
    FILES ${SAGE_APP_ICON_PNG}
)

set_target_properties(SageQt
    PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME ${SAGE_PRODUCT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
        MACOSX_BUNDLE_GUI_IDENTIFIER ${SAGE_BUNDLE_IDENTIFIER}
        MACOSX_BUNDLE_COPYRIGHT ${SAGE_COPYRIGHT}
        MACOSX_BUNDLE_ICON_FILE sageqt.icns
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER ${SAGE_BUNDLE_IDENTIFIER}
)

set(SAGE_LICENSE_FILES
    ${SAGE_FONT_RESOURCE_DIR}/GmarketSansLicense.txt
    ${SAGE_FONT_RESOURCE_DIR}/PretendardLicense.txt
)

if(WIN32)
    configure_file(packaging/windows/SageQt.rc.in ${CMAKE_CURRENT_BINARY_DIR}/SageQt.rc @ONLY)
    target_sources(SageQt PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/SageQt.rc)
endif()

if(APPLE)
    target_sources(SageQt PRIVATE ${SAGE_LICENSE_FILES} ${SAGE_APP_ICON_ICNS})
    set_source_files_properties(${SAGE_LICENSE_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/licenses)
    set_source_files_properties(${SAGE_APP_ICON_ICNS} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
else()
    add_custom_command(TARGET SageQt POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:SageQt>/licenses
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SAGE_LICENSE_FILES} $<TARGET_FILE_DIR:SageQt>/licenses
        VERBATIM
    )
endif()

if(UNIX AND NOT APPLE)
    configure_file(packaging/linux/SageQt.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/SageQt.desktop @ONLY)
endif()

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

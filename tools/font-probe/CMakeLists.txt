qt_add_executable(SageFontProbe
    main.cpp
    SageFontProbe.cpp
    SageFontProbe.h
    SageFontProbeDefine.h
)

target_link_libraries(SageFontProbe
    PRIVATE
        Qt::Gui
)

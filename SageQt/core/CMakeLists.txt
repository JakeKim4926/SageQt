qt_add_library(sage_core STATIC
    core/auth/ISagePasswordHasher.h
    core/auth/ISageUserRepository.h
    core/auth/SageAuthSession.cpp
    core/auth/SageAuthSession.h
    core/auth/SageUserDto.h
    core/auth/SageUserService.cpp
    core/auth/SageUserService.h
    core/workflow/ISageWorkflowHandler.h
    core/workflow/SageResultRow.h
    core/workflow/SageWorkflowRegistry.cpp
    core/workflow/SageWorkflowRegistry.h
    core/workflow/SageWorkflowResponse.cpp
    core/workflow/SageWorkflowResponse.h
    core/workflow/SageWorkflowResultPresenter.cpp
    core/workflow/SageWorkflowResultPresenter.h
    core/workflow/SageWorkflowResultTable.cpp
    core/workflow/SageWorkflowResultTable.h
    core/workflow/SageWorkflowTab.h
    core/workflow/handlers/SageSampleWorkflowHandler.cpp
    core/workflow/handlers/SageSampleWorkflowHandler.h
)

target_include_directories(sage_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(sage_core
    PUBLIC
        Qt::Core
        sage_define
)

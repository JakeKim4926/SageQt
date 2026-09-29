#include "core/workflow/SageWorkflowRunner.h"

#include "SageDefine.h"
#include "core/workflow/ISageWorkflowHandler.h"
#include "core/workflow/SageWorkflowRegistry.h"
#include "core/workflow/SageWorkflowResponse.h"

SageWorkflowRunResult SageWorkflowRunner::run(const SageWorkflowRegistry& registry,
                                              const SageWorkflowRunRequest& request)
{
    SageWorkflowRunResult result;
    result.m_workflowType = request.m_workflowType;
    result.m_taskType = request.m_taskType;
    const ISageWorkflowHandler* handler = registry.findHandler(request.m_workflowType);
    if (handler == nullptr) {
        result.m_response = SageWorkflowResponse::failure(SAGE_REQUEST_UNKNOWN, SAGE_ERROR_CODE_WORKFLOW_NOT_FOUND,
                                                          SAGE_UI_WORKFLOW_NOT_FOUND);
        return result;
    }
    try {
        result.m_response = handler->runTask(
            request.m_taskType, buildPayload(request.m_inputPath, request.m_outputFolder, request.m_selectedRowNums));
    } catch (...) {
        result.m_response = SageWorkflowResponse::failure(
            handler->requestId(request.m_taskType), SAGE_ERROR_CODE_WORKFLOW_EXCEPTION, SAGE_UI_WORKFLOW_EXCEPTION);
    }
    return result;
}

QJsonObject SageWorkflowRunner::buildPayload(const QString& inputPath, const QString& outputFolder,
                                             const QString& rowNums)
{
    QJsonObject payload;
    payload.insert(SAGE_JSON_KEY_INPUT_PATH, inputPath);
    if (!outputFolder.isEmpty()) {
        payload.insert(SAGE_JSON_KEY_OUTPUT_FOLDER, outputFolder);
    }
    if (!rowNums.isEmpty()) {
        payload.insert(SAGE_JSON_KEY_ROW_NUMS, rowNums);
    }
    return payload;
}

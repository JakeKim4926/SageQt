#include "core/workflow/SageWorkflowResponse.h"

#include "SageDefine.h"

#include <QJsonValue>

QJsonObject SageWorkflowResponse::success(const QString& requestId, const QJsonObject& payload)
{
    QJsonObject response;
    response.insert(SAGE_JSON_KEY_TYPE, SAGE_JSON_TYPE_RESPONSE);
    response.insert(SAGE_JSON_KEY_REQUEST_ID, requestId);
    response.insert(SAGE_JSON_KEY_SUCCESS, true);
    response.insert(SAGE_JSON_KEY_PAYLOAD, payload);
    response.insert(SAGE_JSON_KEY_ERROR, QJsonValue::Null);
    return response;
}

QJsonObject SageWorkflowResponse::failure(const QString& requestId, const QString& errorCode,
                                          const QString& errorMessage)
{
    QJsonObject error;
    error.insert(SAGE_JSON_KEY_CODE, errorCode);
    error.insert(SAGE_JSON_KEY_MESSAGE, errorMessage);

    QJsonObject response;
    response.insert(SAGE_JSON_KEY_TYPE, SAGE_JSON_TYPE_RESPONSE);
    response.insert(SAGE_JSON_KEY_REQUEST_ID, requestId);
    response.insert(SAGE_JSON_KEY_SUCCESS, false);
    response.insert(SAGE_JSON_KEY_PAYLOAD, QJsonValue::Null);
    response.insert(SAGE_JSON_KEY_ERROR, error);
    return response;
}

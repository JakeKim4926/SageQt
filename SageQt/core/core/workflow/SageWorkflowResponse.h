#pragma once

#include <QJsonObject>
#include <QString>

class SageWorkflowResponse
{
public:
    static QJsonObject success(const QString& requestId, const QJsonObject& payload);
    static QJsonObject failure(const QString& requestId, const QString& errorCode, const QString& errorMessage);
};

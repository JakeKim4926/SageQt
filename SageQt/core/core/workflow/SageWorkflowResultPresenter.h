#pragma once

#include "SageDefine.h"
#include "core/workflow/SageResultRow.h"

#include <QJsonObject>
#include <QList>
#include <QString>

class ISageWorkflowHandler;

class SageWorkflowResultPresenter
{
public:
    bool buildRows(const ISageWorkflowHandler* handler, SageTaskType taskType, const QJsonObject& response,
                   QList<SageResultRow>& outRows) const;

private:
    static void addRow(QList<SageResultRow>& outRows, const QString& field, const QString& value, const QString& status,
                       const QString& reason);
    static void addSummaryRows(const QJsonObject& payload, QList<SageResultRow>& outRows);
    static QString integerText(const QJsonObject& object, const QString& key);
};

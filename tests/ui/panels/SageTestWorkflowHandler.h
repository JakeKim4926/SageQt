#pragma once

#include "core/workflow/ISageWorkflowHandler.h"
#include "core/workflow/handlers/SageSampleWorkflowHandler.h"

#include <QString>

class SageTestWorkflowHandler final : public ISageWorkflowHandler
{
public:
    SageTestWorkflowHandler(SageWorkflowType workflowType, const QString& label, const QString& category,
                            bool isLoginRequired)
        : m_workflowType(workflowType)
        , m_label(label)
        , m_category(category)
        , m_isLoginRequired(isLoginRequired)
    {
    }

    SageWorkflowType workflowType() const override
    {
        return m_workflowType;
    }
    QString sidebarLabel() const override
    {
        return m_label;
    }
    QString category() const override
    {
        return m_category;
    }
    bool isLoginRequired() const override
    {
        return m_isLoginRequired;
    }

    QString headerTitle() const override
    {
        return m_sample.headerTitle();
    }
    QString inputSectionLabel() const override
    {
        return m_sample.inputSectionLabel();
    }
    QString actionButtonLabel() const override
    {
        return m_sample.actionButtonLabel();
    }
    QList<SageWorkflowTab> tabs() const override
    {
        return m_sample.tabs();
    }
    QList<SageWorkflowColumn> resultColumns(SageTaskType taskType) const override
    {
        return m_sample.resultColumns(taskType);
    }
    SageWorkflowResultStyle resultStyle(SageTaskType taskType) const override
    {
        return m_sample.resultStyle(taskType);
    }
    bool hasCustomResultTable(SageTaskType taskType) const override
    {
        return m_sample.hasCustomResultTable(taskType);
    }
    bool buildResultSummary(SageTaskType taskType, const QList<SageResultRow>& visibleRows, const QJsonObject& response,
                            QList<SageResultSummaryItem>& outItems) const override
    {
        return m_sample.buildResultSummary(taskType, visibleRows, response, outItems);
    }
    bool buildResultTotals(SageTaskType taskType, const QList<SageResultRow>& visibleRows,
                           QList<SageResultTotalCell>& outCells) const override
    {
        return m_sample.buildResultTotals(taskType, visibleRows, outCells);
    }
    QList<SageWorkflowFilterCriteria> filterCriteria() const override
    {
        return m_sample.filterCriteria();
    }
    QString inputDialogTitle() const override
    {
        return m_sample.inputDialogTitle();
    }
    bool hasInputTable() const override
    {
        return m_sample.hasInputTable();
    }
    std::optional<QString> generateCompletedMessage() const override
    {
        return m_sample.generateCompletedMessage();
    }
    bool validateSelectedRows(int selectedCount, bool hasSelectedRowNums, QString& outError) const override
    {
        return m_sample.validateSelectedRows(selectedCount, hasSelectedRowNums, outError);
    }
    QString requestId(SageTaskType taskType) const override
    {
        return m_sample.requestId(taskType);
    }
    QJsonObject runTask(SageTaskType taskType, const QJsonObject& payload) const override
    {
        return m_sample.runTask(taskType, payload);
    }
    bool buildResultRows(SageTaskType taskType, const QJsonObject& response,
                         QList<SageResultRow>& outRows) const override
    {
        return m_sample.buildResultRows(taskType, response, outRows);
    }

private:
    SageWorkflowType m_workflowType;
    QString m_label;
    QString m_category;
    bool m_isLoginRequired;
    SageSampleWorkflowHandler m_sample;
};

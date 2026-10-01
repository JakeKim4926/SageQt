#pragma once

#include "core/workflow/ISageWorkflowHandler.h"
#include "core/workflow/handlers/SageSampleWorkflowHandler.h"

#include <QJsonObject>
#include <QString>

#include <functional>
#include <utility>

class SageTestWorkflowHandler final : public ISageWorkflowHandler
{
public:
    using SageRunTask = std::function<QJsonObject(SageTaskType, const QJsonObject&)>;

    SageTestWorkflowHandler(SageWorkflowType workflowType, const QString& label, const QString& category,
                            bool isLoginRequired)
        : m_workflowType(workflowType)
        , m_label(label)
        , m_category(category)
        , m_isLoginRequired(isLoginRequired)
    {
    }

    void setRunTask(SageRunTask runTask)
    {
        m_runTask = std::move(runTask);
    }
    void setHasInputTable(bool hasInputTable)
    {
        m_hasInputTable = hasInputTable;
    }
    void setCustomResultTable(const SageWorkflowResultStyle& style, const QList<SageWorkflowFilterCriteria>& criteria,
                              const QList<SageResultRow>& rows)
    {
        m_hasCustomResultTable = true;
        m_resultStyle = style;
        m_filterCriteria = criteria;
        m_customRows = rows;
    }
    void setSummaryLabel(const QString& summaryLabel)
    {
        m_summaryLabel = summaryLabel;
    }
    void setSelectionError(const QString& selectionError)
    {
        m_selectionError = selectionError;
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
        return m_hasCustomResultTable ? m_resultStyle : m_sample.resultStyle(taskType);
    }
    bool hasCustomResultTable(SageTaskType taskType) const override
    {
        return m_hasCustomResultTable || m_sample.hasCustomResultTable(taskType);
    }
    bool buildResultSummary(SageTaskType taskType, const QList<SageResultRow>& visibleRows, const QJsonObject& response,
                            QList<SageResultSummaryItem>& outItems) const override
    {
        if (!m_summaryLabel.isEmpty()) {
            outItems = {{m_summaryLabel, QString::number(visibleRows.size()), QString(), false, false}};
            return true;
        }
        return m_sample.buildResultSummary(taskType, visibleRows, response, outItems);
    }
    bool buildResultTotals(SageTaskType taskType, const QList<SageResultRow>& visibleRows,
                           QList<SageResultTotalCell>& outCells) const override
    {
        return m_sample.buildResultTotals(taskType, visibleRows, outCells);
    }
    QList<SageWorkflowFilterCriteria> filterCriteria() const override
    {
        return m_hasCustomResultTable ? m_filterCriteria : m_sample.filterCriteria();
    }
    QString inputDialogTitle() const override
    {
        return m_sample.inputDialogTitle();
    }
    QString inputFileFilter() const override
    {
        return m_sample.inputFileFilter();
    }
    bool hasInputTable() const override
    {
        return m_hasInputTable;
    }
    std::optional<QString> generateCompletedMessage() const override
    {
        return m_sample.generateCompletedMessage();
    }
    bool validateSelectedRows(int selectedCount, bool hasSelectedRowNums, QString& outError) const override
    {
        if (!m_selectionError.isEmpty() && (selectedCount == 0 || !hasSelectedRowNums)) {
            outError = m_selectionError;
            return false;
        }
        return m_sample.validateSelectedRows(selectedCount, hasSelectedRowNums, outError);
    }
    QString requestId(SageTaskType taskType) const override
    {
        return m_sample.requestId(taskType);
    }
    QJsonObject runTask(SageTaskType taskType, const QJsonObject& payload) const override
    {
        if (m_runTask) {
            return m_runTask(taskType, payload);
        }
        return m_sample.runTask(taskType, payload);
    }
    bool buildResultRows(SageTaskType taskType, const QJsonObject& response,
                         QList<SageResultRow>& outRows) const override
    {
        if (m_hasCustomResultTable) {
            outRows = m_customRows;
            return true;
        }
        return m_sample.buildResultRows(taskType, response, outRows);
    }

private:
    SageWorkflowType m_workflowType;
    QString m_label;
    QString m_category;
    bool m_isLoginRequired;
    bool m_hasInputTable = false;
    bool m_hasCustomResultTable = false;
    SageWorkflowResultStyle m_resultStyle;
    QList<SageWorkflowFilterCriteria> m_filterCriteria;
    QList<SageResultRow> m_customRows;
    QString m_summaryLabel;
    QString m_selectionError;
    SageRunTask m_runTask;
    SageSampleWorkflowHandler m_sample;
};

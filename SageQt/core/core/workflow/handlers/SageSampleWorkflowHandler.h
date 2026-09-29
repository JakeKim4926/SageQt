#pragma once

#include "core/workflow/ISageWorkflowHandler.h"

class SageSampleWorkflowHandler final : public ISageWorkflowHandler
{
public:
    SageWorkflowType workflowType() const override;

    QString sidebarLabel() const override;
    QString category() const override;

    QString headerTitle() const override;
    QString inputSectionLabel() const override;
    QString actionButtonLabel() const override;

    QList<SageWorkflowTab> tabs() const override;

    QList<SageWorkflowColumn> resultColumns(SageTaskType taskType) const override;
    SageWorkflowResultStyle resultStyle(SageTaskType taskType) const override;
    bool hasCustomResultTable(SageTaskType taskType) const override;
    bool buildResultSummary(SageTaskType taskType, const QList<SageResultRow>& visibleRows, const QJsonObject& response,
                            QList<SageResultSummaryItem>& outItems) const override;
    bool buildResultTotals(SageTaskType taskType, const QList<SageResultRow>& visibleRows,
                           QList<SageResultTotalCell>& outCells) const override;

    QList<SageWorkflowFilterCriteria> filterCriteria() const override;

    QString inputDialogTitle() const override;
    QString inputFileFilter() const override;
    bool hasInputTable() const override;
    std::optional<QString> generateCompletedMessage() const override;

    bool validateSelectedRows(int selectedCount, bool hasSelectedRowNums, QString& outError) const override;

    bool isLoginRequired() const override;

    QString requestId(SageTaskType taskType) const override;
    QJsonObject runTask(SageTaskType taskType, const QJsonObject& payload) const override;
    bool buildResultRows(SageTaskType taskType, const QJsonObject& response,
                         QList<SageResultRow>& outRows) const override;
};

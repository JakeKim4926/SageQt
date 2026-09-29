#pragma once

#include "SageDefine.h"
#include "core/workflow/SageResultRow.h"
#include "core/workflow/SageWorkflowResultTable.h"
#include "core/workflow/SageWorkflowTab.h"

#include <QJsonObject>
#include <QList>
#include <QString>

#include <optional>

class ISageWorkflowHandler
{
public:
    virtual ~ISageWorkflowHandler() = default;

    virtual SageWorkflowType workflowType() const = 0;

    virtual QString sidebarLabel() const = 0;
    virtual QString category() const = 0;

    virtual QString headerTitle() const = 0;
    virtual QString inputSectionLabel() const = 0;
    virtual QString actionButtonLabel() const = 0;

    virtual QList<SageWorkflowTab> tabs() const = 0;

    virtual QList<SageWorkflowColumn> resultColumns(SageTaskType taskType) const = 0;
    virtual SageWorkflowResultStyle resultStyle(SageTaskType taskType) const = 0;
    virtual bool hasCustomResultTable(SageTaskType taskType) const = 0;
    virtual bool buildResultSummary(SageTaskType taskType, const QList<SageResultRow>& visibleRows,
                                    const QJsonObject& response, QList<SageResultSummaryItem>& outItems) const = 0;
    virtual bool buildResultTotals(SageTaskType taskType, const QList<SageResultRow>& visibleRows,
                                   QList<SageResultTotalCell>& outCells) const = 0;

    virtual QList<SageWorkflowFilterCriteria> filterCriteria() const = 0;

    virtual QString inputDialogTitle() const = 0;
    virtual QString inputFileFilter() const = 0;
    virtual bool hasInputTable() const = 0;
    virtual std::optional<QString> generateCompletedMessage() const = 0;

    virtual bool validateSelectedRows(int selectedCount, bool hasSelectedRowNums, QString& outError) const = 0;

    virtual bool isLoginRequired() const = 0;

    virtual QString requestId(SageTaskType taskType) const = 0;
    virtual QJsonObject runTask(SageTaskType taskType, const QJsonObject& payload) const = 0;
    virtual bool buildResultRows(SageTaskType taskType, const QJsonObject& response,
                                 QList<SageResultRow>& outRows) const = 0;
};

#pragma once

#include "core/workflow/SageWorkflowRunRequest.h"
#include "core/workflow/SageWorkflowRunResult.h"

#include <QFutureWatcher>
#include <QObject>
#include <QString>

class SageWorkflowRegistry;

class SageWorkflowController : public QObject
{
    Q_OBJECT

public:
    explicit SageWorkflowController(const SageWorkflowRegistry& registry, QObject* parent = nullptr);

    bool isRunning() const;
    bool start(const SageWorkflowRunRequest& request, QString& outError);
    void finish(const SageWorkflowRunResult& result, bool success, bool keepResult);

    const SageWorkflowResultState& resultState() const;
    void clearResult();
    void restoreResult(const SageWorkflowResultState& state);

signals:
    void runFinished(const SageWorkflowRunResult& result);

private slots:
    void onWatcherFinished();

private:
    const SageWorkflowRegistry& m_registry;
    QFutureWatcher<SageWorkflowRunResult>* m_watcher = nullptr;
    bool m_running = false;
    SageWorkflowResultState m_resultState;
};

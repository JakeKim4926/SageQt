#include "ui/workflow/SageWorkflowController.h"

#include "SageDefine.h"
#include "core/workflow/SageWorkflowRunner.h"

#include <QtConcurrentRun>

SageWorkflowController::SageWorkflowController(const SageWorkflowRegistry& registry, QObject* parent)
    : QObject(parent)
    , m_registry(registry)
    , m_watcher(new QFutureWatcher<SageWorkflowRunResult>(this))
{
    connect(m_watcher, &QFutureWatcher<SageWorkflowRunResult>::finished, this,
            &SageWorkflowController::onWatcherFinished);
}

bool SageWorkflowController::isRunning() const
{
    return m_isRunning;
}

bool SageWorkflowController::start(const SageWorkflowRunRequest& request, QString& outError)
{
    if (m_isRunning) {
        outError = SAGE_UI_WORKFLOW_ALREADY_RUNNING;
        return false;
    }
    m_resultState.m_inputPath = request.m_inputPath;
    m_isRunning = true;
    m_watcher->setFuture(
        QtConcurrent::run([&registry = m_registry, request]() { return SageWorkflowRunner::run(registry, request); }));
    return true;
}

void SageWorkflowController::finish(const SageWorkflowRunResult& result, bool success, bool keepResult)
{
    m_isRunning = false;
    m_resultState.m_workflowType = result.m_workflowType;
    m_resultState.m_isSuccessful = success;
    if (keepResult) {
        return;
    }
    m_resultState.m_taskType = result.m_taskType;
    m_resultState.m_response = result.m_response;
}

const SageWorkflowResultState& SageWorkflowController::resultState() const
{
    return m_resultState;
}

void SageWorkflowController::clearResult()
{
    m_resultState = SageWorkflowResultState();
}

void SageWorkflowController::restoreResult(const SageWorkflowResultState& state)
{
    m_resultState = state;
}

void SageWorkflowController::onWatcherFinished()
{
    emit runFinished(m_watcher->result());
}

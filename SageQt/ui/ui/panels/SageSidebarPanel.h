#pragma once

#include "SageDefine.h"

#include <QItemSelection>
#include <QPersistentModelIndex>
#include <QWidget>

#include <optional>

class QFrame;
class QTreeView;
class SageAuthSession;
class SageLabel;
class SageSidebarModel;
class SageSurface;
class SageWorkflowRegistry;

class SageSidebarPanel : public QWidget
{
    Q_OBJECT

public:
    SageSidebarPanel(const SageWorkflowRegistry& registry, const SageAuthSession& authSession,
                     QWidget* parent = nullptr);

    std::optional<SageWorkflowType> selectedWorkflow() const;

signals:
    void workflowSelected(SageWorkflowType workflowType);
    void passwordChangeRequested();

private slots:
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:
    void createWidgets(const SageWorkflowRegistry& registry);
    void createLayout();
    void connectSignals();
    void selectFirstWorkflow();
    void restoreLastWorkflow();

private:
    const SageAuthSession& m_authSession;
    SageSurface* m_surface = nullptr;
    SageLabel* m_titleLabel = nullptr;
    QFrame* m_titleDivider = nullptr;
    SageSidebarModel* m_sidebarModel = nullptr;
    QTreeView* m_workflowTree = nullptr;
    QPersistentModelIndex m_lastWorkflowIndex;
    std::optional<SageWorkflowType> m_selectedWorkflow;
};

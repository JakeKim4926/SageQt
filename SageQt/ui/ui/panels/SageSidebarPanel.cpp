#include "ui/panels/SageSidebarPanel.h"

#include "core/auth/SageAuthSession.h"
#include "core/workflow/SageWorkflowRegistry.h"
#include "ui/dialogs/SageMessageBoxDlg.h"
#include "ui/models/SageSidebarModel.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/widgets/SageLabel.h"
#include "ui/widgets/SageSidebarDelegate.h"
#include "ui/widgets/SageSurface.h"

#include <QAbstractItemView>
#include <QFrame>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QTreeView>
#include <QVBoxLayout>

SageSidebarPanel::SageSidebarPanel(const SageWorkflowRegistry& registry, const SageAuthSession& authSession,
                                   QWidget* parent)
    : QWidget(parent)
    , m_authSession(authSession)
{
    setFixedWidth(SAGE_SIDEBAR_WIDTH);
    createWidgets(registry);
    createLayout();
    connectSignals();
    selectFirstWorkflow();
}

std::optional<SageWorkflowType> SageSidebarPanel::selectedWorkflow() const
{
    return m_selectedWorkflow;
}

void SageSidebarPanel::onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(deselected)
    if (selected.indexes().isEmpty()) {
        restoreLastWorkflow();
        return;
    }
    const QModelIndex index = selected.indexes().constFirst();
    const SageSidebarItemKind kind =
        index.data(static_cast<int>(SageSidebarRole::ItemKind)).value<SageSidebarItemKind>();
    if (kind == SageSidebarItemKind::Group) {
        return;
    }

    if (index.data(static_cast<int>(SageSidebarRole::LoginRequired)).toBool() && !m_authSession.isLoggedIn()) {
        SageMessageBoxDlg warningDialog(SageMessageIcon::Warning, SAGE_UI_LOGIN_REQUIRED, this);
        warningDialog.exec();
        restoreLastWorkflow();
        return;
    }

    if (kind == SageSidebarItemKind::ChangePassword) {
        emit passwordChangeRequested();
        restoreLastWorkflow();
        return;
    }

    const SageWorkflowType workflowType =
        index.data(static_cast<int>(SageSidebarRole::WorkflowType)).value<SageWorkflowType>();
    m_lastWorkflowIndex = index;
    if (m_selectedWorkflow == workflowType) {
        return;
    }
    m_selectedWorkflow = workflowType;
    emit workflowSelected(workflowType);
}

void SageSidebarPanel::createWidgets(const SageWorkflowRegistry& registry)
{
    m_surface = new SageSurface(SageSurface::SageSurfaceVariant::Sidebar, this);

    m_titleLabel = new SageLabel(SageLabel::SageLabelVariant::SidebarLogo, SAGE_UI_APP_TITLE, m_surface);
    m_titleLabel->setFixedHeight(SAGE_HEADER_HEIGHT);
    m_titleLabel->setIndent(SAGE_SIDEBAR_PAD_X);
    m_titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    m_titleDivider = new QFrame(m_surface);
    m_titleDivider->setFrameShape(QFrame::HLine);
    m_titleDivider->setFixedHeight(SAGE_BORDER_THICKNESS);

    m_sidebarModel = new SageSidebarModel(registry.handlers(), this);
    m_workflowTree = new QTreeView(m_surface);
    m_workflowTree->setModel(m_sidebarModel);
    m_workflowTree->setItemDelegate(new SageSidebarDelegate(m_workflowTree));
    m_workflowTree->setHeaderHidden(true);
    m_workflowTree->setRootIsDecorated(false);
    m_workflowTree->setItemsExpandable(false);
    m_workflowTree->setIndentation(0);
    m_workflowTree->setFrameShape(QFrame::NoFrame);
    m_workflowTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_workflowTree->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_workflowTree->setSelectionMode(QAbstractItemView::SingleSelection);
    m_workflowTree->expandAll();
}

void SageSidebarPanel::createLayout()
{
    QVBoxLayout* panelLayout = new QVBoxLayout(this);
    panelLayout->setContentsMargins(0, 0, 0, 0);
    panelLayout->addWidget(m_surface);

    QVBoxLayout* surfaceLayout = new QVBoxLayout(m_surface);
    surfaceLayout->setContentsMargins(0, 0, 0, SAGE_MARGIN);
    surfaceLayout->setSpacing(0);
    surfaceLayout->addWidget(m_titleLabel);
    surfaceLayout->addWidget(m_titleDivider);
    surfaceLayout->addSpacing(SAGE_SIDEBAR_TREE_TOP_PAD - SAGE_BORDER_THICKNESS);
    surfaceLayout->addWidget(m_workflowTree);
}

void SageSidebarPanel::connectSignals()
{
    connect(m_workflowTree->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            &SageSidebarPanel::onSelectionChanged);
}

void SageSidebarPanel::selectFirstWorkflow()
{
    const QModelIndex firstWorkflow = m_sidebarModel->firstWorkflowIndex();
    if (!firstWorkflow.isValid()) {
        return;
    }
    m_lastWorkflowIndex = firstWorkflow;
    m_selectedWorkflow = firstWorkflow.data(static_cast<int>(SageSidebarRole::WorkflowType)).value<SageWorkflowType>();
    m_workflowTree->setCurrentIndex(firstWorkflow);
}

void SageSidebarPanel::restoreLastWorkflow()
{
    if (m_lastWorkflowIndex.isValid()) {
        m_workflowTree->setCurrentIndex(m_lastWorkflowIndex);
    }
}

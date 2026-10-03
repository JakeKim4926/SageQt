#include "ui/models/SageSidebarModel.h"

#include "SageDefine.h"
#include "core/workflow/ISageWorkflowHandler.h"

#include <QHash>
#include <QStandardItem>
#include <QVariant>

SageSidebarModel::SageSidebarModel(const QList<const ISageWorkflowHandler*>& handlers, QObject* parent)
    : QStandardItemModel(parent)
{
    QHash<QString, QStandardItem*> groups;
    for (const ISageWorkflowHandler* handler : handlers) {
        QStandardItem* group = groups.value(handler->category(), nullptr);
        if (group == nullptr) {
            group = addGroup(handler->category());
            groups.insert(handler->category(), group);
        }
        addWorkflow(*group, *handler);
    }

    QStandardItem* etcGroup = addGroup(SAGE_UI_SIDEBAR_GROUP_ETC);
    addChangePassword(*etcGroup);
}

QModelIndex SageSidebarModel::firstWorkflowIndex() const
{
    for (int groupRow = 0; groupRow < rowCount(); ++groupRow) {
        const QStandardItem* group = item(groupRow);
        for (int row = 0; row < group->rowCount(); ++row) {
            const QStandardItem* child = group->child(row);
            if (child->data(static_cast<int>(SageSidebarRole::ItemKind)).value<SageSidebarItemKind>() ==
                SageSidebarItemKind::Workflow) {
                return child->index();
            }
        }
    }
    return {};
}

QStandardItem* SageSidebarModel::addGroup(const QString& label)
{
    QStandardItem* group = new QStandardItem(label);
    appendRow(group);
    group->setFlags(Qt::ItemIsEnabled);
    group->setData(QVariant::fromValue(SageSidebarItemKind::Group), static_cast<int>(SageSidebarRole::ItemKind));
    return group;
}

void SageSidebarModel::addWorkflow(QStandardItem& group, const ISageWorkflowHandler& handler)
{
    QStandardItem* workflow = new QStandardItem(handler.sidebarLabel());
    group.appendRow(workflow);
    workflow->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    workflow->setData(QVariant::fromValue(SageSidebarItemKind::Workflow), static_cast<int>(SageSidebarRole::ItemKind));
    workflow->setData(QVariant::fromValue(handler.workflowType()), static_cast<int>(SageSidebarRole::WorkflowType));
    workflow->setData(handler.isLoginRequired(), static_cast<int>(SageSidebarRole::LoginRequired));
}

void SageSidebarModel::addChangePassword(QStandardItem& group)
{
    QStandardItem* changePassword = new QStandardItem(SAGE_UI_CHANGE_PW_MENU);
    group.appendRow(changePassword);
    changePassword->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    changePassword->setData(QVariant::fromValue(SageSidebarItemKind::ChangePassword),
                            static_cast<int>(SageSidebarRole::ItemKind));
    changePassword->setData(true, static_cast<int>(SageSidebarRole::LoginRequired));
}

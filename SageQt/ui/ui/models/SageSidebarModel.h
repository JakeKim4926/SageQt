#pragma once

#include <QList>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QString>

class ISageWorkflowHandler;
class QStandardItem;

enum class SageSidebarRole
{
    ItemKind = Qt::UserRole,
    WorkflowType,
    LoginRequired
};

enum class SageSidebarItemKind
{
    Group,
    Workflow,
    ChangePassword
};

class SageSidebarModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit SageSidebarModel(const QList<const ISageWorkflowHandler*>& handlers, QObject* parent = nullptr);

    QModelIndex firstWorkflowIndex() const;

private:
    QStandardItem* addGroup(const QString& label);
    static void addWorkflow(QStandardItem& group, const ISageWorkflowHandler& handler);
    static void addChangePassword(QStandardItem& group);
};

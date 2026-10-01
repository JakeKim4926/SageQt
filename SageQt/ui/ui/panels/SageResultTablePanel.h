#pragma once

#include "core/workflow/SageResultRow.h"
#include "core/workflow/SageWorkflowResultTable.h"

#include <QList>
#include <QString>
#include <QWidget>

class QEvent;
class QFrame;
class QTableView;
class SageLabel;
class SageResultFilterProxyModel;
class SageResultTableDelegate;
class SageResultTableModel;

class SageResultTablePanel : public QWidget
{
    Q_OBJECT

public:
    explicit SageResultTablePanel(const QString& title, QWidget* parent = nullptr);

    void setColumns(const QList<SageWorkflowColumn>& columns, const SageWorkflowResultStyle& style);
    void setRows(const QList<SageResultRow>& rows);
    void clearRows();
    int rowCount() const;

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void createWidgets(const QString& title);
    void createLayout();
    void connectSignals();
    void setHoveredRow(int row);
    void applyColumnWidths();
    static int columnWidth(SageResultField field);

private:
    SageLabel* m_titleLabel = nullptr;
    QFrame* m_titleLine = nullptr;
    QTableView* m_tableView = nullptr;
    SageResultTableModel* m_model = nullptr;
    SageResultFilterProxyModel* m_proxy = nullptr;
    SageResultTableDelegate* m_delegate = nullptr;
};

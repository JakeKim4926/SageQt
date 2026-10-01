#pragma once

#include "core/workflow/SageResultRow.h"
#include "core/workflow/SageWorkflowResultTable.h"

#include <QList>
#include <QString>
#include <QWidget>

class QEvent;
class QFrame;
class QHBoxLayout;
class QSpacerItem;
class QTableView;
class QVBoxLayout;
class SageButton;
class SageLabel;
class SageResultFilterProxyModel;
class SageResultTableDelegate;
class SageResultTableModel;
class SageSearchBox;
class SageSelectionBar;
class SageSummaryBar;
class SageTableTotalBar;

class SageResultTablePanel : public QWidget
{
    Q_OBJECT

public:
    explicit SageResultTablePanel(QWidget* parent = nullptr);
    explicit SageResultTablePanel(const QString& title, QWidget* parent = nullptr);

    void showSelectAll(bool visible);
    void showFilter(bool visible);
    void setSelectionControlsEnabled(bool enabled);

    void setColumns(const QList<SageWorkflowColumn>& columns, const SageWorkflowResultStyle& style);
    void setFilterCriteria(const QList<SageWorkflowFilterCriteria>& criteria);
    void setRows(const QList<SageResultRow>& rows);
    void clearRows();
    QList<SageResultRow> visibleRows() const;

    void setSummaryItems(const QList<SageResultSummaryItem>& items);
    void clearSummary();
    void setTotalCells(const QList<SageResultTotalCell>& cells);
    void clearTotals();

    int rowCount() const;
    int checkedRowCount() const;
    QString checkedRowNums() const;
    void restoreCheckedRowNums(const QString& checkedRowNums);

    QString filterKeyword() const;
    int filterCriteria() const;
    void restoreFilter(const QString& keyword, int criteria);

signals:
    void filterChanged();
    void selectionChanged(int selectedCount);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void createWidgets(const QString& title);
    void createLayout();
    void connectSignals();
    void onSearchRequested();
    void onFilterReset();
    void onCriteriaChanged(int criteria);
    void onSelectAllClicked();
    void onClearClicked();
    void onCheckStateChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles);
    void applyFilter(const QString& keyword, int criteria);
    void populateCriteria();
    void setAllRowsChecked(bool checked);
    void notifySelectionChanged();
    void syncSelectionBar();
    void updateBand();
    void updateTotalBarCells();
    void setHoveredRow(int row);
    void applyColumnWidths();
    static int columnWidth(SageResultField field);

private:
    QWidget* m_band = nullptr;
    QWidget* m_titleBlock = nullptr;
    SageLabel* m_titleLabel = nullptr;
    QFrame* m_titleLine = nullptr;
    SageSelectionBar* m_selectionBar = nullptr;
    SageSummaryBar* m_summaryBar = nullptr;
    SageButton* m_resetButton = nullptr;
    SageSearchBox* m_searchBox = nullptr;
    QHBoxLayout* m_bandLayout = nullptr;
    QSpacerItem* m_filterGap = nullptr;
    QSpacerItem* m_resetGap = nullptr;
    QVBoxLayout* m_layout = nullptr;
    QSpacerItem* m_bandGap = nullptr;
    QTableView* m_tableView = nullptr;
    SageTableTotalBar* m_totalBar = nullptr;
    SageResultTableModel* m_model = nullptr;
    SageResultFilterProxyModel* m_proxy = nullptr;
    SageResultTableDelegate* m_delegate = nullptr;
    QList<SageResultTotalCell> m_totalCells;
    bool m_hasTitle = false;
    bool m_selectAllVisible = false;
    bool m_filterVisible = false;
    bool m_updatingChecks = false;
};

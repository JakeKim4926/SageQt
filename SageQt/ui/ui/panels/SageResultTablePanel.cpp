#include "ui/panels/SageResultTablePanel.h"

#include "SageDefine.h"
#include "ui/models/SageResultFilterProxyModel.h"
#include "ui/models/SageResultTableModel.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/widgets/SageButton.h"
#include "ui/widgets/SageLabel.h"
#include "ui/widgets/SageSearchBox.h"
#include "ui/widgets/SageSelectionBar.h"
#include "ui/widgets/SageSummaryBar.h"
#include "ui/widgets/SageTableTotalBar.h"
#include "ui/widgets/SageTableView.h"

#include <QApplication>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QScrollBar>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QStyle>
#include <QVBoxLayout>

SageResultTablePanel::SageResultTablePanel(QWidget* parent)
    : QWidget(parent)
{
    createWidgets(QString());
    createLayout();
    connectSignals();
    updateBand();
}

SageResultTablePanel::SageResultTablePanel(const QString& title, QWidget* parent)
    : QWidget(parent)
    , m_hasTitle(true)
{
    createWidgets(title);
    createLayout();
    connectSignals();
    updateBand();
}

void SageResultTablePanel::showSelectAll(bool visible)
{
    m_selectAllVisible = visible;
    if (visible) {
        syncSelectionBar();
    }
    updateBand();
}

void SageResultTablePanel::showFilter(bool visible)
{
    m_filterVisible = visible;
    if (visible) {
        populateCriteria();
    }
    updateBand();
}

void SageResultTablePanel::setSelectionControlsEnabled(bool enabled)
{
    m_selectionBar->setControlsEnabled(enabled);
}

void SageResultTablePanel::setColumns(const QList<SageWorkflowColumn>& columns, const SageWorkflowResultStyle& style)
{
    m_model->setColumns(columns, style);
    m_tableView->setRowSeparator(style.m_hasGridLines);
    QList<SageTableColumnSpec> columnSpecs;
    columnSpecs.reserve(columns.size());
    for (const SageWorkflowColumn& column : columns) {
        columnSpecs.append({columnWidth(column.m_field), column.m_isStretch});
    }
    m_tableView->setColumnSpecs(columnSpecs);
}

void SageResultTablePanel::setFilterCriteria(const QList<SageWorkflowFilterCriteria>& criteria)
{
    m_proxy->setFilterCriteria(criteria);
    if (m_filterVisible) {
        populateCriteria();
    }
}

void SageResultTablePanel::setRows(const QList<SageResultRow>& rows)
{
    if (m_filterVisible) {
        populateCriteria();
    }
    m_model->setRows(rows);
}

void SageResultTablePanel::clearRows()
{
    m_model->clearRows();
}

QList<SageResultRow> SageResultTablePanel::visibleRows() const
{
    return m_proxy->visibleRows();
}

void SageResultTablePanel::setSummaryItems(const QList<SageResultSummaryItem>& items)
{
    m_summaryBar->setItems(items);
    updateBand();
}

void SageResultTablePanel::clearSummary()
{
    setSummaryItems({});
}

void SageResultTablePanel::setTotalCells(const QList<SageResultTotalCell>& cells)
{
    m_totalCells = cells;
    m_totalBar->setVisible(!m_totalCells.isEmpty());
    updateTotalBarCells();
}

void SageResultTablePanel::clearTotals()
{
    setTotalCells({});
}

int SageResultTablePanel::rowCount() const
{
    return m_proxy->rowCount();
}

int SageResultTablePanel::checkedRowCount() const
{
    return m_proxy->checkedRowCount();
}

QString SageResultTablePanel::checkedRowNums() const
{
    return m_proxy->checkedRowNums();
}

void SageResultTablePanel::restoreCheckedRowNums(const QString& checkedRowNums)
{
    if (checkedRowNums.isEmpty()) {
        return;
    }
    m_updatingChecks = true;
    m_proxy->restoreCheckedRowNums(checkedRowNums);
    m_updatingChecks = false;
    notifySelectionChanged();
}

QString SageResultTablePanel::filterKeyword() const
{
    return m_proxy->keyword();
}

int SageResultTablePanel::filterCriteria() const
{
    return m_proxy->criteria();
}

void SageResultTablePanel::restoreFilter(const QString& keyword, int criteria)
{
    m_searchBox->setKeyword(keyword);
    applyFilter(keyword, criteria);
}

void SageResultTablePanel::createWidgets(const QString& title)
{
    m_band = new QWidget(this);
    m_band->setMinimumHeight(SAGE_BUTTON_VERT_ADJUST + SAGE_RESULT_HEADER_HEIGHT);
    m_titleBlock = new QWidget(m_band);
    m_titleBlock->setFixedHeight(SAGE_BUTTON_VERT_ADJUST + SAGE_RESULT_HEADER_HEIGHT);
    m_titleLabel = new SageLabel(SageLabel::SageLabelVariant::TableTitle, title, m_titleBlock);
    m_titleLabel->setFixedHeight(SAGE_RESULT_HEADER_HEIGHT - SAGE_BORDER_THICKNESS);
    m_titleLabel->setIndent(SAGE_CARD_PADDING);
    m_titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_titleLine = new QFrame(m_titleBlock);
    m_titleLine->setFrameShape(QFrame::HLine);
    m_titleLine->setFixedHeight(SAGE_BORDER_THICKNESS);
    m_selectionBar = new SageSelectionBar(m_band);
    m_summaryBar = new SageSummaryBar(m_band);
    m_resetButton = new SageButton(SAGE_UI_RESULT_RESET_BTN, m_band);
    m_resetButton->setVariant(SageButton::SageButtonVariant::Ghost);
    m_resetButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
    m_resetButton->setFixedWidth(SAGE_RESULT_RESET_WIDTH);
    m_searchBox = new SageSearchBox(m_band);

    m_model = new SageResultTableModel(this);
    m_proxy = new SageResultFilterProxyModel(*m_model, this);
    m_tableView = new SageTableView(this);
    m_tableView->setModel(m_proxy);
    m_tableView->setMinimumHeight(SAGE_RESULT_MIN_HEIGHT);
    m_totalBar = new SageTableTotalBar(this);
    m_totalBar->hide();
}

void SageResultTablePanel::createLayout()
{
    QVBoxLayout* titleLayout = new QVBoxLayout(m_titleBlock);
    titleLayout->setContentsMargins(0, SAGE_BUTTON_VERT_ADJUST, 0, 0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(m_titleLabel);
    titleLayout->addWidget(m_titleLine);

    QWidget* bandLeft = new QWidget(m_band);
    bandLeft->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QHBoxLayout* bandLeftLayout = new QHBoxLayout(bandLeft);
    bandLeftLayout->setContentsMargins(0, 0, 0, 0);
    bandLeftLayout->setSpacing(0);
    bandLeftLayout->addWidget(m_titleBlock, 1, Qt::AlignTop);
    bandLeftLayout->addWidget(m_selectionBar, 1, Qt::AlignLeft | Qt::AlignTop);
    bandLeftLayout->addWidget(m_summaryBar, 1, Qt::AlignTop);

    m_bandLayout = new QHBoxLayout(m_band);
    m_bandLayout->setContentsMargins(0, 0, 0, 0);
    m_bandLayout->setSpacing(0);
    m_bandLayout->addWidget(bandLeft, 1);
    m_filterGap = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);
    m_bandLayout->addItem(m_filterGap);
    m_bandLayout->addWidget(m_resetButton, 0, Qt::AlignTop);
    m_resetGap = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);
    m_bandLayout->addItem(m_resetGap);
    m_bandLayout->addWidget(m_searchBox, 0, Qt::AlignTop);

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, SAGE_RESULT_FILTER_TOP_LIFT + SAGE_RESULT_FILTER_BOX_PAD - SAGE_BUTTON_VERT_ADJUST,
                                 0, 0);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_band);
    m_bandGap = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Fixed);
    m_layout->addItem(m_bandGap);
    m_layout->addWidget(m_tableView, 1);
    m_layout->addWidget(m_totalBar);
}

void SageResultTablePanel::connectSignals()
{
    connect(m_searchBox, &SageSearchBox::searchRequested, this, &SageResultTablePanel::onSearchRequested);
    connect(m_searchBox, &SageSearchBox::criteriaChanged, this, &SageResultTablePanel::onCriteriaChanged);
    connect(m_resetButton, &SageButton::clicked, this, &SageResultTablePanel::onFilterReset);
    connect(m_selectionBar, &SageSelectionBar::selectAllClicked, this, &SageResultTablePanel::onSelectAllClicked);
    connect(m_selectionBar, &SageSelectionBar::clearClicked, this, &SageResultTablePanel::onClearClicked);
    connect(m_model, &SageResultTableModel::dataChanged, this, &SageResultTablePanel::onCheckStateChanged);
    connect(m_model, &SageResultTableModel::modelReset, this, &SageResultTablePanel::syncSelectionBar);
    connect(m_tableView->horizontalHeader(), &QHeaderView::sectionResized, this,
            &SageResultTablePanel::updateTotalBarCells);
    connect(m_tableView->horizontalScrollBar(), &QScrollBar::valueChanged, this,
            &SageResultTablePanel::updateTotalBarCells);
}

void SageResultTablePanel::onSearchRequested()
{
    applyFilter(m_searchBox->keyword().trimmed(), m_proxy->criteria());
    emit filterChanged();
}

void SageResultTablePanel::onFilterReset()
{
    m_searchBox->setKeyword(QString());
    applyFilter(QString(), m_proxy->criteria());
    emit filterChanged();
}

void SageResultTablePanel::onCriteriaChanged(int criteria)
{
    applyFilter(m_proxy->keyword(), criteria);
    emit filterChanged();
}

void SageResultTablePanel::onSelectAllClicked()
{
    const int totalCount = m_proxy->rowCount();
    const bool allChecked = totalCount > 0 && m_proxy->checkedRowCount() == totalCount;
    setAllRowsChecked(!allChecked);
    notifySelectionChanged();
}

void SageResultTablePanel::onClearClicked()
{
    setAllRowsChecked(false);
    notifySelectionChanged();
}

void SageResultTablePanel::onCheckStateChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight,
                                               const QList<int>& roles)
{
    Q_UNUSED(topLeft)
    Q_UNUSED(bottomRight)
    if (m_updatingChecks || !roles.contains(Qt::CheckStateRole)) {
        return;
    }
    notifySelectionChanged();
}

void SageResultTablePanel::applyFilter(const QString& keyword, int criteria)
{
    m_updatingChecks = true;
    m_proxy->setFilter(keyword, criteria);
    m_updatingChecks = false;
    if (m_filterVisible) {
        populateCriteria();
    }
    syncSelectionBar();
}

void SageResultTablePanel::populateCriteria()
{
    m_searchBox->setCriteria(m_proxy->filterCriteria(), m_proxy->effectiveCriteria());
}

void SageResultTablePanel::setAllRowsChecked(bool checked)
{
    m_updatingChecks = true;
    m_proxy->setAllRowsChecked(checked);
    m_updatingChecks = false;
}

void SageResultTablePanel::notifySelectionChanged()
{
    syncSelectionBar();
    emit selectionChanged(m_proxy->checkedRowCount());
}

void SageResultTablePanel::syncSelectionBar()
{
    const int totalCount = m_proxy->rowCount();
    const int selectedCount = m_proxy->checkedRowCount();
    m_selectionBar->setCounts(totalCount, selectedCount);
    m_selectionBar->setAllChecked(totalCount > 0 && selectedCount == totalCount);
}

void SageResultTablePanel::updateBand()
{
    const bool selectionVisible = m_selectAllVisible;
    const bool summaryVisible = !selectionVisible && m_summaryBar->hasItems();
    m_selectionBar->setVisible(selectionVisible);
    m_summaryBar->setVisible(summaryVisible);
    m_titleBlock->setVisible(!selectionVisible && !summaryVisible && m_hasTitle);
    m_resetButton->setVisible(m_filterVisible);
    m_searchBox->setVisible(m_filterVisible);
    m_filterGap->changeSize(m_filterVisible ? SAGE_ROW_GAP : 0, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);
    m_resetGap->changeSize(m_filterVisible ? SAGE_ACTION_GAP : 0, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);
    m_bandGap->changeSize(0, selectionVisible || summaryVisible ? SAGE_ROW_GAP : 0, QSizePolicy::Minimum,
                          QSizePolicy::Fixed);
    m_bandLayout->invalidate();
    m_layout->invalidate();
}

void SageResultTablePanel::updateTotalBarCells()
{
    const QHeaderView* header = m_tableView->horizontalHeader();
    const int tableLeft = m_tableView->geometry().left() + m_tableView->frameWidth();
    QList<SageTableTotalBarCell> barCells;
    for (const SageResultTotalCell& cell : std::as_const(m_totalCells)) {
        if (cell.m_column < 0 || cell.m_column >= m_model->columnCount()) {
            continue;
        }
        barCells.append({cell.m_text, tableLeft + header->sectionViewportPosition(cell.m_column),
                         header->sectionSize(cell.m_column), m_model->column(cell.m_column).m_align, cell.m_role});
    }
    m_totalBar->setCells(barCells);
}

int SageResultTablePanel::columnWidth(SageResultField field)
{
    switch (field) {
    case SageResultField::Field:
        return SAGE_RESULT_FIELD_WIDTH;
    case SageResultField::Value:
        return SAGE_RESULT_MIN_VALUE_WIDTH;
    case SageResultField::Status:
        return SAGE_RESULT_STATUS_WIDTH;
    case SageResultField::Reason:
        return SAGE_RESULT_REASON_WIDTH;
    }
    return SAGE_RESULT_FIELD_WIDTH;
}

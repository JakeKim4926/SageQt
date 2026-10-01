#include "ui/panels/SageResultTablePanel.h"

#include "ui/models/SageResultFilterProxyModel.h"
#include "ui/models/SageResultTableModel.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/widgets/SageLabel.h"
#include "ui/widgets/SageResultTableDelegate.h"

#include <QAbstractItemView>
#include <QEvent>
#include <QFrame>
#include <QHeaderView>
#include <QTableView>
#include <QVBoxLayout>

SageResultTablePanel::SageResultTablePanel(const QString& title, QWidget* parent)
    : QWidget(parent)
{
    createWidgets(title);
    createLayout();
    connectSignals();
}

void SageResultTablePanel::setColumns(const QList<SageWorkflowColumn>& columns, const SageWorkflowResultStyle& style)
{
    m_model->setColumns(columns, style);
    m_delegate->setRowSeparator(style.m_hasGridLines);
    applyColumnWidths();
}

void SageResultTablePanel::setRows(const QList<SageResultRow>& rows)
{
    m_model->setRows(rows);
}

void SageResultTablePanel::clearRows()
{
    m_model->clearRows();
}

int SageResultTablePanel::rowCount() const
{
    return m_proxy->rowCount();
}

void SageResultTablePanel::createWidgets(const QString& title)
{
    m_titleLabel = new SageLabel(SageLabel::SageLabelVariant::TableTitle, title, this);
    m_titleLabel->setFixedHeight(SAGE_RESULT_HEADER_HEIGHT - SAGE_BORDER_THICKNESS);
    m_titleLabel->setIndent(SAGE_CARD_PADDING);
    m_titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_titleLine = new QFrame(this);
    m_titleLine->setFrameShape(QFrame::HLine);
    m_titleLine->setFixedHeight(SAGE_BORDER_THICKNESS);

    m_model = new SageResultTableModel(this);
    m_proxy = new SageResultFilterProxyModel(*m_model, this);
    m_delegate = new SageResultTableDelegate(this);
    m_tableView = new QTableView(this);
    m_tableView->setModel(m_proxy);
    m_tableView->setItemDelegate(m_delegate);
    m_tableView->setFrameShape(QFrame::Box);
    m_tableView->setFrameShadow(QFrame::Plain);
    m_tableView->setLineWidth(SAGE_BORDER_THICKNESS);
    m_tableView->setShowGrid(false);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->setWordWrap(false);
    m_tableView->setMouseTracking(true);
    m_tableView->setMinimumHeight(SAGE_RESULT_MIN_HEIGHT);
    m_tableView->verticalHeader()->hide();
    m_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_tableView->verticalHeader()->setDefaultSectionSize(SAGE_LIST_ROW_HEIGHT);
    m_tableView->horizontalHeader()->setSectionsClickable(false);
    m_tableView->horizontalHeader()->setHighlightSections(false);
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_tableView->viewport()->installEventFilter(this);
}

bool SageResultTablePanel::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_tableView->viewport() && event->type() == QEvent::Resize) {
        applyColumnWidths();
    }
    if (watched == m_tableView->viewport() && event->type() == QEvent::Leave) {
        setHoveredRow(-1);
    }
    return QWidget::eventFilter(watched, event);
}

void SageResultTablePanel::createLayout()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, SAGE_RESULT_FILTER_TOP_LIFT + SAGE_RESULT_FILTER_BOX_PAD, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_titleLine);
    layout->addWidget(m_tableView, 1);
}

void SageResultTablePanel::connectSignals()
{
    connect(m_tableView, &QTableView::entered, this, [this](const QModelIndex& index) { setHoveredRow(index.row()); });
    connect(m_proxy, &SageResultFilterProxyModel::modelReset, this, [this]() { setHoveredRow(-1); });
    connect(m_proxy, &SageResultFilterProxyModel::rowsInserted, this, [this]() { setHoveredRow(-1); });
    connect(m_proxy, &SageResultFilterProxyModel::rowsRemoved, this, [this]() { setHoveredRow(-1); });
}

void SageResultTablePanel::setHoveredRow(int row)
{
    m_delegate->setHoveredRow(row);
    m_tableView->viewport()->update();
}

void SageResultTablePanel::applyColumnWidths()
{
    QHeaderView* header = m_tableView->horizontalHeader();
    const int columnCount = m_model->columnCount();
    int fixedWidth = 0;
    int stretchMinimumWidth = 0;
    int lastStretchColumn = -1;
    for (int column = 0; column < columnCount; ++column) {
        const SageWorkflowColumn& definition = m_model->column(column);
        if (definition.m_isStretch) {
            stretchMinimumWidth += columnWidth(definition.m_field);
            lastStretchColumn = column;
            continue;
        }
        fixedWidth += columnWidth(definition.m_field);
    }
    const int stretchWidth = m_tableView->viewport()->width() - fixedWidth;
    const bool fitsStretch = stretchMinimumWidth > 0 && stretchWidth >= stretchMinimumWidth;
    int assignedStretchWidth = 0;
    for (int column = 0; column < columnCount; ++column) {
        const SageWorkflowColumn& definition = m_model->column(column);
        int width = columnWidth(definition.m_field);
        if (definition.m_isStretch && fitsStretch) {
            width = column == lastStretchColumn ? stretchWidth - assignedStretchWidth
                                                : width * stretchWidth / stretchMinimumWidth;
            assignedStretchWidth += width;
        }
        header->resizeSection(column, width);
    }
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

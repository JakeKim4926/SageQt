#include "ui/widgets/SageTableView.h"

#include "ui/style/SageDesignDefine.h"
#include "ui/widgets/SageResultTableDelegate.h"

#include <QAbstractItemModel>
#include <QEvent>
#include <QFrame>
#include <QHeaderView>

SageTableView::SageTableView(QWidget* parent)
    : QTableView(parent)
{
    m_delegate = new SageResultTableDelegate(this);
    setItemDelegate(m_delegate);
    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Plain);
    setLineWidth(SAGE_BORDER_THICKNESS);
    setShowGrid(false);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setWordWrap(false);
    setMouseTracking(true);
    verticalHeader()->hide();
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(SAGE_LIST_ROW_HEIGHT);
    horizontalHeader()->setSectionsClickable(false);
    horizontalHeader()->setHighlightSections(false);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    connect(this, &SageTableView::entered, this, [this](const QModelIndex& index) { setHoveredRow(index.row()); });
}

void SageTableView::setColumnSpecs(const QList<SageTableColumnSpec>& columnSpecs)
{
    m_columnSpecs = columnSpecs;
    applyColumnWidths();
}

void SageTableView::setRowSeparator(bool enabled)
{
    m_delegate->setRowSeparator(enabled);
    viewport()->update();
}

void SageTableView::setModel(QAbstractItemModel* model)
{
    QTableView::setModel(model);
    if (model == nullptr) {
        return;
    }
    connect(model, &QAbstractItemModel::modelReset, this, [this]() { setHoveredRow(-1); });
    connect(model, &QAbstractItemModel::rowsInserted, this, [this]() { setHoveredRow(-1); });
    connect(model, &QAbstractItemModel::rowsRemoved, this, [this]() { setHoveredRow(-1); });
}

bool SageTableView::viewportEvent(QEvent* event)
{
    if (event->type() == QEvent::Resize) {
        applyColumnWidths();
    }
    if (event->type() == QEvent::Leave) {
        setHoveredRow(-1);
    }
    return QTableView::viewportEvent(event);
}

void SageTableView::applyColumnWidths()
{
    const int columnCount = qMin(static_cast<int>(m_columnSpecs.size()), horizontalHeader()->count());
    int fixedWidth = 0;
    int stretchMinimumWidth = 0;
    int lastStretchColumn = -1;
    for (int column = 0; column < columnCount; ++column) {
        const SageTableColumnSpec& spec = m_columnSpecs.at(column);
        if (spec.m_isStretch) {
            stretchMinimumWidth += spec.m_width;
            lastStretchColumn = column;
            continue;
        }
        fixedWidth += spec.m_width;
    }
    const int stretchWidth = viewport()->width() - fixedWidth;
    const bool fitsStretch = stretchMinimumWidth > 0 && stretchWidth >= stretchMinimumWidth;
    int assignedStretchWidth = 0;
    for (int column = 0; column < columnCount; ++column) {
        const SageTableColumnSpec& spec = m_columnSpecs.at(column);
        int width = spec.m_width;
        if (spec.m_isStretch && fitsStretch) {
            width = column == lastStretchColumn ? stretchWidth - assignedStretchWidth
                                                : spec.m_width * stretchWidth / stretchMinimumWidth;
            assignedStretchWidth += width;
        }
        horizontalHeader()->resizeSection(column, width);
    }
}

void SageTableView::setHoveredRow(int row)
{
    m_delegate->setHoveredRow(row);
    viewport()->update();
}

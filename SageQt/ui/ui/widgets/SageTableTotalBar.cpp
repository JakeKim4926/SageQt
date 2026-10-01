#include "ui/widgets/SageTableTotalBar.h"

#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageFontCatalog.h"

#include <QFont>
#include <QPainter>
#include <QRect>
#include <QSizePolicy>

#include <utility>

SageTableTotalBar::SageTableTotalBar(QWidget* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void SageTableTotalBar::setCells(const QList<SageTableTotalBarCell>& cells)
{
    m_cells = cells;
    update();
}

QSize SageTableTotalBar::sizeHint() const
{
    return {0, SAGE_TOTAL_BAR_HEIGHT};
}

QSize SageTableTotalBar::minimumSizeHint() const
{
    return sizeHint();
}

void SageTableTotalBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(), SAGE_COLOR_LIST_HEADER);
    painter.fillRect(QRect(0, 0, width(), SAGE_BORDER_THICKNESS), SAGE_COLOR_BORDER);
    for (const SageTableTotalBarCell& cell : std::as_const(m_cells)) {
        if (cell.m_text.isEmpty() || cell.m_width <= 0) {
            continue;
        }
        QRect cellRect(cell.m_left, 0, cell.m_width, height());
        Qt::Alignment alignment = Qt::AlignVCenter;
        if (cell.m_align == SageColumnAlign::Center) {
            alignment |= Qt::AlignHCenter;
        } else if (cell.m_align == SageColumnAlign::Right) {
            cellRect.setRight(cellRect.right() - SAGE_LIST_CELL_RIGHT_PAD);
            alignment |= Qt::AlignRight;
        } else {
            cellRect.setLeft(cellRect.left() + SAGE_LIST_CELL_LEFT_PAD);
            alignment |= Qt::AlignLeft;
        }
        painter.setFont(SageFontCatalog::font(cell.m_role == SageResultTotalRole::Count ? SageFontRole::ListStrong
                                                                                        : SageFontRole::ListBold));
        painter.setPen(cellColor(cell.m_role));
        painter.drawText(cellRect, alignment, cell.m_text);
    }
}

QColor SageTableTotalBar::cellColor(SageResultTotalRole role)
{
    switch (role) {
    case SageResultTotalRole::Label:
        return SAGE_COLOR_TEXT_MUTED;
    case SageResultTotalRole::Count:
        return SAGE_COLOR_SECONDARY_TEXT;
    case SageResultTotalRole::AmountHighlight:
        return SAGE_COLOR_PRIMARY;
    case SageResultTotalRole::Amount:
        return SAGE_COLOR_TEXT;
    }
    return SAGE_COLOR_TEXT;
}

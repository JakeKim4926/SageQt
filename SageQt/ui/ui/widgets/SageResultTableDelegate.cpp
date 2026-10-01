#include "ui/widgets/SageResultTableDelegate.h"

#include "ui/models/SageTableRole.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageFontCatalog.h"

#include <QAbstractItemModel>
#include <QEvent>
#include <QFont>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QSize>
#include <QStyle>
#include <QStyleOptionViewItem>

#include <iterator>

SageResultTableDelegate::SageResultTableDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

void SageResultTableDelegate::setRowSeparator(bool enabled)
{
    m_rowSeparator = enabled;
}

void SageResultTableDelegate::setHoveredRow(int row)
{
    m_hoveredRow = row;
}

void SageResultTableDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                                    const QModelIndex& index) const
{
    const QRect cellRect = option.rect;
    const bool selected = option.state.testFlag(QStyle::State_Selected);
    QColor background = index.row() % 2 == 1 ? SAGE_COLOR_LIST_ROW_ALT : SAGE_COLOR_PANEL;
    if (index.data(static_cast<int>(SageTableRole::RowTone)).value<SageTableTone>() == SageTableTone::Failed) {
        background = SAGE_COLOR_STATUS_CARD_BG_ERROR;
    }
    if (index.row() == m_hoveredRow) {
        background = SAGE_COLOR_LIST_ROW_HOVER;
    }
    if (selected) {
        background = SAGE_COLOR_LIST_ROW_SELECTED;
    }
    painter->fillRect(cellRect, background);

    if (hasCheckBox(index)) {
        drawCheckBox(painter, cellRect, index.data(Qt::CheckStateRole).value<Qt::CheckState>() == Qt::Checked);
    }
    const SageTableTone badgeTone = index.data(static_cast<int>(SageTableRole::BadgeTone)).value<SageTableTone>();
    if (badgeTone != SageTableTone::None) {
        drawBadge(painter, cellRect, index.data().toString(), badgeTone);
    }
    const bool highlighted = index.data(static_cast<int>(SageTableRole::Highlighted)).toBool();
    const QFont font = SageFontCatalog::font(highlighted ? SageFontRole::ListBold : SageFontRole::List);
    const QRect textArea = textRect(cellRect, index);
    painter->setFont(font);
    painter->setPen(textColor(index));
    if (badgeTone == SageTableTone::None) {
        painter->drawText(textArea, index.data(Qt::TextAlignmentRole).value<Qt::Alignment>(),
                          QFontMetrics(font).elidedText(index.data().toString(), Qt::ElideRight, textArea.width()));
    }

    if (m_rowSeparator) {
        painter->fillRect(QRect(cellRect.left(), cellRect.bottom() + 1 - SAGE_LIST_GRID_THICKNESS, cellRect.width(),
                                SAGE_LIST_GRID_THICKNESS),
                          SAGE_COLOR_LIST_GRID);
    }
    if (selected && index.column() == 0) {
        painter->fillRect(QRect(cellRect.left(), cellRect.top(), SAGE_LIST_SELECTION_ACCENT_WIDTH, cellRect.height()),
                          SAGE_COLOR_PRIMARY);
    }
}

QSize SageResultTableDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return {QStyledItemDelegate::sizeHint(option, index).width(), SAGE_LIST_ROW_HEIGHT};
}

bool SageResultTableDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option,
                                          const QModelIndex& index)
{
    if (!hasCheckBox(index) || !index.flags().testFlag(Qt::ItemIsEnabled)) {
        return false;
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        const QMouseEvent* mouseEvent = static_cast<const QMouseEvent*>(event);
        if (mouseEvent->button() != Qt::LeftButton ||
            !checkCellRect(option.rect).contains(mouseEvent->position().toPoint())) {
            return false;
        }
    } else if (event->type() == QEvent::KeyPress) {
        const int key = static_cast<const QKeyEvent*>(event)->key();
        if (key != Qt::Key_Space && key != Qt::Key_Select) {
            return false;
        }
    } else {
        return false;
    }
    const bool checked = index.data(Qt::CheckStateRole).value<Qt::CheckState>() == Qt::Checked;
    return model->setData(index, checked ? Qt::Unchecked : Qt::Checked, Qt::CheckStateRole);
}

bool SageResultTableDelegate::hasCheckBox(const QModelIndex& index)
{
    return index.flags().testFlag(Qt::ItemIsUserCheckable);
}

QRect SageResultTableDelegate::checkCellRect(const QRect& cellRect)
{
    const int checkLeft = cellRect.left() + SAGE_LIST_SELECTION_ACCENT_WIDTH + SAGE_LIST_CHECK_ACCENT_GAP;
    return {checkLeft, cellRect.top(), cellRect.left() + SAGE_LIST_CHECK_CELL_WIDTH - checkLeft, cellRect.height()};
}

QRect SageResultTableDelegate::textRect(const QRect& cellRect, const QModelIndex& index)
{
    if (index.column() != 0) {
        return cellRect.adjusted(SAGE_LIST_CELL_LEFT_PAD, 0, -SAGE_LIST_CELL_RIGHT_PAD, 0);
    }
    if (!hasCheckBox(index)) {
        return cellRect;
    }
    return cellRect.adjusted(SAGE_LIST_CHECK_CELL_WIDTH, 0, -SAGE_LIST_CHECK_CELL_WIDTH, 0);
}

void SageResultTableDelegate::drawCheckBox(QPainter* painter, const QRect& cellRect, bool checked)
{
    const QRect checkCell = checkCellRect(cellRect);
    QRect box(0, 0, SAGE_LIST_CHECK_BOX_SIZE, SAGE_LIST_CHECK_BOX_SIZE);
    box.moveTopLeft({checkCell.left() + (checkCell.width() - SAGE_LIST_CHECK_BOX_SIZE) / 2,
                     checkCell.top() + (checkCell.height() - SAGE_LIST_CHECK_BOX_SIZE) / 2});
    if (!checked) {
        painter->fillRect(box, SAGE_COLOR_PANEL);
        painter->setPen(QPen(SAGE_COLOR_BUTTON_BORDER, SAGE_BORDER_THICKNESS));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(box.adjusted(0, 0, -SAGE_BORDER_THICKNESS, -SAGE_BORDER_THICKNESS));
        return;
    }
    painter->fillRect(box, SAGE_COLOR_PRIMARY);
    const int inset = SAGE_LIST_CHECK_MARK_THICKNESS * 2;
    const int boxRight = box.left() + box.width();
    const int boxBottom = box.top() + box.height();
    const QPointF points[] = {
        QPointF(box.left() + inset, box.top() + box.height() / 2),
        QPointF(box.left() + box.width() / 2, boxBottom - inset),
        QPointF(boxRight - inset, box.top() + inset),
    };
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(SAGE_COLOR_PANEL, SAGE_LIST_CHECK_MARK_THICKNESS));
    painter->drawPolyline(points, std::size(points));
    painter->restore();
}

void SageResultTableDelegate::drawBadge(QPainter* painter, const QRect& contentRect, const QString& text,
                                        SageTableTone tone)
{
    if (text.isEmpty()) {
        return;
    }
    const QFont font = SageFontCatalog::font(SageFontRole::Caption);
    const int badgeWidth = QFontMetrics(font).horizontalAdvance(text) + SAGE_LIST_BADGE_PAD_X * 2;
    const QRect badge(contentRect.left() + (contentRect.width() - badgeWidth) / 2,
                      contentRect.top() + (contentRect.height() - SAGE_LIST_BADGE_HEIGHT) / 2, badgeWidth,
                      SAGE_LIST_BADGE_HEIGHT);
    const QColor face = tone == SageTableTone::Success ? SAGE_COLOR_BADGE_BG_SUCCESS : SAGE_COLOR_STATUS_BG_ERROR;
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(face);
    painter->drawRoundedRect(badge, SAGE_LIST_BADGE_RADIUS, SAGE_LIST_BADGE_RADIUS);
    painter->restore();
    painter->setFont(font);
    painter->setPen(tone == SageTableTone::Success ? SAGE_COLOR_STATUS_CARD_TEXT_SUCCESS
                                                   : SAGE_COLOR_INLINE_ERROR_TEXT);
    painter->drawText(badge, Qt::AlignCenter, text);
}

QColor SageResultTableDelegate::textColor(const QModelIndex& index)
{
    if (index.column() == 0) {
        return SAGE_COLOR_TEXT;
    }
    if (index.data(static_cast<int>(SageTableRole::Muted)).toBool()) {
        return SAGE_COLOR_TEXT_PLACEHOLDER;
    }
    if (index.data(static_cast<int>(SageTableRole::Highlighted)).toBool()) {
        return SAGE_COLOR_PRIMARY;
    }
    return SAGE_COLOR_TEXT;
}

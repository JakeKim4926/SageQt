#include "ui/widgets/SageEmptyState.h"

#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageFontCatalog.h"

#include <QBrush>
#include <QFontMetrics>
#include <QPainter>
#include <QPen>
#include <qdrawutil.h>

SageEmptyState::SageEmptyState(QWidget* parent)
    : QWidget(parent)
{
}

void SageEmptyState::setContent(const QString& title, const QString& description)
{
    m_title = title;
    m_description = description;
    update();
}

QString SageEmptyState::title() const
{
    return m_title;
}

QString SageEmptyState::description() const
{
    return m_description;
}

void SageEmptyState::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    const QBrush face(SAGE_COLOR_PANEL);
    qDrawPlainRect(&painter, rect(), SAGE_COLOR_BORDER, SAGE_BORDER_THICKNESS, &face);

    const int width = textWidth();
    const int textLeft = (this->width() - width) / 2;
    const int descHeight = descriptionHeight(width);
    int blockHeight = SAGE_EMPTY_ICON_BOX_SIZE + SAGE_EMPTY_BLOCK_GAP + SAGE_EMPTY_TITLE_HEIGHT;
    if (descHeight > 0) {
        blockHeight += SAGE_EMPTY_BLOCK_GAP + descHeight;
    }
    int top = (height() - blockHeight) / 2;

    drawIconBox(painter, QRect(this->width() / 2 - SAGE_EMPTY_ICON_BOX_SIZE / 2, top, SAGE_EMPTY_ICON_BOX_SIZE,
                               SAGE_EMPTY_ICON_BOX_SIZE));
    top += SAGE_EMPTY_ICON_BOX_SIZE + SAGE_EMPTY_BLOCK_GAP;

    painter.setFont(SageFontCatalog::font(SageFontRole::Section));
    painter.setPen(SAGE_COLOR_TEXT);
    painter.drawText(QRect(textLeft, top, width, SAGE_EMPTY_TITLE_HEIGHT), Qt::AlignCenter, m_title);
    top += SAGE_EMPTY_TITLE_HEIGHT;
    if (descHeight <= 0) {
        return;
    }
    top += SAGE_EMPTY_BLOCK_GAP;
    painter.setFont(SageFontCatalog::font(SageFontRole::Body));
    painter.setPen(SAGE_COLOR_SECONDARY_TEXT);
    painter.drawText(QRect(textLeft, top, width, descHeight), Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap,
                     m_description);
}

int SageEmptyState::textWidth() const
{
    return qMin(width(), SAGE_EMPTY_DESC_MAX_WIDTH);
}

int SageEmptyState::descriptionHeight(int width) const
{
    if (m_description.isEmpty()) {
        return 0;
    }
    return QFontMetrics(SageFontCatalog::font(SageFontRole::Body))
        .boundingRect(QRect(0, 0, width, 0), Qt::AlignHCenter | Qt::TextWordWrap, m_description)
        .height();
}

void SageEmptyState::drawIconBox(QPainter& painter, const QRect& box)
{
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(SAGE_COLOR_LIST_HEADER);
    painter.drawRoundedRect(box, SAGE_EMPTY_ICON_BOX_RADIUS, SAGE_EMPTY_ICON_BOX_RADIUS);
    painter.restore();

    const QRect icon(box.left() + (box.width() - SAGE_EMPTY_ICON_SIZE) / 2,
                     box.top() + (box.height() - SAGE_EMPTY_ICON_SIZE) / 2, SAGE_EMPTY_ICON_SIZE, SAGE_EMPTY_ICON_SIZE);
    const QRect grid = icon.adjusted(SAGE_EMPTY_ICON_INSET_X, SAGE_EMPTY_ICON_INSET_Y, -SAGE_EMPTY_ICON_INSET_X,
                                     -SAGE_EMPTY_ICON_INSET_Y);
    painter.setPen(QPen(SAGE_COLOR_PRIMARY, SAGE_BORDER_THICKNESS));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(grid.adjusted(0, 0, -SAGE_BORDER_THICKNESS, -SAGE_BORDER_THICKNESS));
    const int headerY = grid.top() + SAGE_EMPTY_ICON_HEADER_OFFSET;
    painter.drawLine(grid.left(), headerY, grid.right() - SAGE_BORDER_THICKNESS, headerY);
    const int dividerX = grid.left() + SAGE_EMPTY_ICON_DIVIDER_OFFSET;
    painter.drawLine(dividerX, headerY, dividerX, grid.bottom() - SAGE_BORDER_THICKNESS);
}

#include "ui/widgets/SageSummaryBar.h"

#include "SageDefine.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageFontCatalog.h"
#include "ui/style/SageStyleDefine.h"

#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QPen>
#include <QRect>
#include <QRectF>
#include <QSizePolicy>

SageSummaryBar::SageSummaryBar(QWidget* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void SageSummaryBar::setItems(const QList<SageResultSummaryItem>& items)
{
    m_items = items;
    update();
}

bool SageSummaryBar::hasItems() const
{
    return !m_items.isEmpty();
}

QSize SageSummaryBar::sizeHint() const
{
    return {0, SAGE_SUMMARY_BAR_HEIGHT};
}

QSize SageSummaryBar::minimumSizeHint() const
{
    return sizeHint();
}

void SageSummaryBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    const QFont captionFont = SageFontCatalog::font(SageFontRole::Caption);
    const QFont summaryFont = SageFontCatalog::font(SageFontRole::Summary);
    const int badgeIndex = firstBadgeIndex();
    int left = 0;
    int badgeLeft = -1;
    for (int index = 0; index < m_items.size(); ++index) {
        if (index > 0) {
            left += SAGE_SUMMARY_ITEM_GAP;
            painter.fillRect(QRect(left, (height() - SAGE_SUMMARY_DIVIDER_HEIGHT) / 2, SAGE_BORDER_THICKNESS,
                                   SAGE_SUMMARY_DIVIDER_HEIGHT),
                             SAGE_COLOR_BORDER);
            left += SAGE_BORDER_THICKNESS + SAGE_SUMMARY_ITEM_GAP;
        }
        const SageResultSummaryItem& item = m_items.at(index);
        if (index == badgeIndex) {
            badgeLeft = left;
        }
        const int width = itemWidth(item);
        if (left + width > this->width()) {
            break;
        }
        if (item.m_isBadge) {
            left += width;
            continue;
        }
        left = drawSegment(painter, left, item.m_label, captionFont, SAGE_COLOR_SECONDARY_TEXT);
        if (!item.m_label.isEmpty()) {
            left += SAGE_SUMMARY_TEXT_GAP;
        }
        left = drawSegment(painter, left, item.m_value, summaryFont,
                           item.m_isHighlighted ? SAGE_COLOR_PRIMARY : SAGE_COLOR_TEXT);
        if (item.m_unit.isEmpty()) {
            continue;
        }
        left += SAGE_SUMMARY_TEXT_GAP;
        left = drawSegment(painter, left, item.m_unit, captionFont, SAGE_COLOR_SECONDARY_TEXT);
    }
    if (badgeIndex >= 0 && badgeLeft >= 0) {
        drawBadge(painter, badgeLeft, m_items.at(badgeIndex));
    }
}

int SageSummaryBar::itemWidth(const SageResultSummaryItem& item) const
{
    const QFontMetrics captionMetrics(SageFontCatalog::font(SageFontRole::Caption));
    if (item.m_isBadge) {
        return captionMetrics.horizontalAdvance(badgeText(item)) + SAGE_BADGE_PAD_X * 2;
    }
    int width = item.m_label.isEmpty() ? 0 : captionMetrics.horizontalAdvance(item.m_label) + SAGE_SUMMARY_TEXT_GAP;
    width += QFontMetrics(SageFontCatalog::font(SageFontRole::Summary)).horizontalAdvance(item.m_value);
    if (!item.m_unit.isEmpty()) {
        width += SAGE_SUMMARY_TEXT_GAP + captionMetrics.horizontalAdvance(item.m_unit);
    }
    return width;
}

int SageSummaryBar::drawSegment(QPainter& painter, int left, const QString& text, const QFont& font,
                                const QColor& color) const
{
    if (text.isEmpty()) {
        return left;
    }
    const int width = QFontMetrics(font).horizontalAdvance(text);
    painter.setFont(font);
    painter.setPen(color);
    painter.drawText(QRect(left, 0, width, height()), Qt::AlignLeft | Qt::AlignVCenter, text);
    return left + width;
}

void SageSummaryBar::drawBadge(QPainter& painter, int left, const SageResultSummaryItem& item) const
{
    const QRect badge(left, (height() - SAGE_BADGE_HEIGHT) / 2, itemWidth(item), SAGE_BADGE_HEIGHT);
    const double inset = SAGE_BORDER_THICKNESS * SAGE_STROKE_CENTER_OFFSET;
    const double radius = SAGE_BADGE_RADIUS * SAGE_ROUND_RECT_DIAMETER_TO_RADIUS;
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(SAGE_COLOR_INLINE_WARN_BORDER, SAGE_BORDER_THICKNESS));
    painter.setBrush(SAGE_COLOR_INLINE_WARN_BG);
    painter.drawRoundedRect(QRectF(badge).adjusted(inset, inset, -inset, -inset), radius, radius);
    painter.restore();
    painter.setFont(SageFontCatalog::font(SageFontRole::Caption));
    painter.setPen(SAGE_COLOR_WARNING);
    painter.drawText(badge, Qt::AlignCenter, badgeText(item));
}

QString SageSummaryBar::badgeText(const SageResultSummaryItem& item)
{
    return SAGE_UI_SUMMARY_BADGE_FORMAT.arg(item.m_label, item.m_value, item.m_unit);
}

int SageSummaryBar::firstBadgeIndex() const
{
    for (int index = 0; index < m_items.size(); ++index) {
        if (m_items.at(index).m_isBadge) {
            return index;
        }
    }
    return -1;
}

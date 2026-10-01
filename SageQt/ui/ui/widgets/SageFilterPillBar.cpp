#include "ui/widgets/SageFilterPillBar.h"

#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageFontCatalog.h"
#include "ui/style/SageStyleDefine.h"

#include <QFontMetrics>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QRectF>
#include <QSizePolicy>

SageFilterPillBar::SageFilterPillBar(QWidget* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void SageFilterPillBar::setLabels(const QStringList& labels)
{
    m_labels = labels;
    if (m_selectedIndex >= m_labels.size()) {
        m_selectedIndex = 0;
    }
    update();
}

int SageFilterPillBar::selectedIndex() const
{
    return m_selectedIndex;
}

QSize SageFilterPillBar::sizeHint() const
{
    return {0, SAGE_PILL_HEIGHT};
}

QSize SageFilterPillBar::minimumSizeHint() const
{
    return sizeHint();
}

void SageFilterPillBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(SageFontCatalog::font(SageFontRole::Caption));
    const QList<QRect> rects = pillRects();
    const double inset = SAGE_BORDER_THICKNESS * SAGE_STROKE_CENTER_OFFSET;
    for (int index = 0; index < rects.size(); ++index) {
        const bool selected = index == m_selectedIndex;
        const QRectF pill = QRectF(rects.at(index)).adjusted(inset, inset, -inset, -inset);
        painter.setPen(QPen(selected ? SAGE_COLOR_PRIMARY : SAGE_COLOR_BORDER, SAGE_BORDER_THICKNESS));
        painter.setBrush(selected ? SAGE_COLOR_ACCENT_SURFACE : SAGE_COLOR_PANEL);
        painter.drawRoundedRect(pill, SAGE_PILL_RADIUS, SAGE_PILL_RADIUS);
        painter.setPen(selected ? SAGE_COLOR_PRIMARY : SAGE_COLOR_TEXT_MUTED);
        painter.drawText(rects.at(index), Qt::AlignCenter, m_labels.at(index));
    }
}

void SageFilterPillBar::mousePressEvent(QMouseEvent* event)
{
    const QList<QRect> rects = pillRects();
    for (int index = 0; index < rects.size(); ++index) {
        if (!rects.at(index).contains(event->position().toPoint()) || index == m_selectedIndex) {
            continue;
        }
        m_selectedIndex = index;
        update();
        emit selectedIndexChanged(index);
        return;
    }
}

QList<QRect> SageFilterPillBar::pillRects() const
{
    const QFontMetrics metrics(SageFontCatalog::font(SageFontRole::Caption));
    QList<QRect> rects;
    int left = 0;
    const int top = (height() - SAGE_PILL_HEIGHT) / 2;
    for (const QString& label : m_labels) {
        const int width = metrics.horizontalAdvance(label) + SAGE_PILL_PAD_X * 2;
        rects.append(QRect(left, top, width, SAGE_PILL_HEIGHT));
        left += width + SAGE_PILL_GAP;
    }
    return rects;
}

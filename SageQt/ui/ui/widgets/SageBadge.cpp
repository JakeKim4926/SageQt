#include "ui/widgets/SageBadge.h"

#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageFontCatalog.h"
#include "ui/style/SageStyleDefine.h"

#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QPen>
#include <QRectF>
#include <QSizePolicy>

SageBadge::SageBadge(SageBadgeVariant variant, QWidget* parent)
    : QWidget(parent)
    , m_variant(variant)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

SageBadge::SageBadgeVariant SageBadge::variant() const
{
    return m_variant;
}

QString SageBadge::text() const
{
    return m_text;
}

void SageBadge::setText(const QString& text)
{
    if (m_text == text) {
        return;
    }
    m_text = text;
    updateGeometry();
    update();
}

QSize SageBadge::sizeHint() const
{
    const int textWidth = QFontMetrics(SageFontCatalog::font(SageFontRole::Caption)).horizontalAdvance(m_text);
    return {textWidth + SAGE_BADGE_PAD_X + SAGE_BADGE_PAD_X, SAGE_BADGE_HEIGHT};
}

QSize SageBadge::minimumSizeHint() const
{
    return sizeHint();
}

void SageBadge::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    if (m_text.isEmpty()) {
        return;
    }
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const double inset = SAGE_BORDER_THICKNESS * SAGE_STROKE_CENTER_OFFSET;
    const QRectF pill = QRectF(rect()).adjusted(inset, inset, -inset, -inset);
    const double radius = pill.height() * SAGE_ROUND_RECT_DIAMETER_TO_RADIUS;
    painter.setPen(QPen(SAGE_COLOR_LIST_HEADER_BORDER, SAGE_BORDER_THICKNESS));
    painter.setBrush(SAGE_COLOR_LIST_HEADER);
    painter.drawRoundedRect(pill, radius, radius);

    painter.setFont(SageFontCatalog::font(SageFontRole::Caption));
    painter.setPen(SAGE_COLOR_PRIMARY);
    painter.drawText(rect(), Qt::AlignCenter, m_text);
}

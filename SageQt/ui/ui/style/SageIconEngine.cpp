#include "ui/style/SageIconEngine.h"

#include "ui/style/SageDesignDefine.h"

#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QSizeF>

SageIconEngine::SageIconEngine(SageIconGlyph glyph)
    : m_glyph(glyph)
{
}

void SageIconEngine::paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state)
{
    Q_UNUSED(state)
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    if (m_glyph == SageIconGlyph::Close) {
        paintClose(painter, rect, mode);
    } else {
        paintMessage(painter, rect);
    }
    painter->restore();
}

QPixmap SageIconEngine::pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state)
{
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    paint(&painter, QRect(QPoint(), size), mode, state);
    return pixmap;
}

QPixmap SageIconEngine::scaledPixmap(const QSize& size, QIcon::Mode mode, QIcon::State state, qreal scale)
{
    QPixmap pixmap(size * scale);
    pixmap.setDevicePixelRatio(scale);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    paint(&painter, QRect(QPoint(), size), mode, state);
    return pixmap;
}

QIconEngine* SageIconEngine::clone() const
{
    return new SageIconEngine(m_glyph);
}

void SageIconEngine::paintClose(QPainter* painter, const QRect& rect, QIcon::Mode mode) const
{
    const QColor color = mode == QIcon::Disabled ? SAGE_COLOR_BORDER : SAGE_COLOR_TEXT_MUTED;
    painter->setPen(QPen(color, SAGE_ICON_STROKE, Qt::SolidLine, Qt::FlatCap));

    QRectF cross(QPointF(), QSizeF(SAGE_ICON_CLOSE_SPAN, SAGE_ICON_CLOSE_SPAN));
    cross.moveCenter(QRectF(rect).center());
    painter->drawLine(cross.topLeft(), cross.bottomRight());
    painter->drawLine(cross.topRight(), cross.bottomLeft());
}

void SageIconEngine::paintMessage(QPainter* painter, const QRect& rect) const
{
    const QColor color = messageColor();
    painter->setPen(QPen(color, SAGE_ICON_STROKE, Qt::SolidLine, Qt::FlatCap));

    const QPointF center = QRectF(rect).center();
    painter->drawEllipse(center, SAGE_MSGBOX_ICON_RADIUS, SAGE_MSGBOX_ICON_RADIUS);

    const bool isInfo = m_glyph == SageIconGlyph::Info;
    const int stemTop = isInfo ? SAGE_MSGBOX_INFO_STEM_TOP : SAGE_MSGBOX_ALERT_STEM_TOP;
    const int stemBottom = isInfo ? SAGE_MSGBOX_INFO_STEM_BOTTOM : SAGE_MSGBOX_ALERT_STEM_BOTTOM;
    const int dotTop = isInfo ? SAGE_MSGBOX_INFO_DOT_TOP : SAGE_MSGBOX_ALERT_DOT_TOP;

    painter->drawLine(QPointF(center.x(), rect.top() + stemTop), QPointF(center.x(), rect.top() + stemBottom));
    QRectF dot(QPointF(), QSizeF(SAGE_MSGBOX_ICON_DOT_SIZE, SAGE_MSGBOX_ICON_DOT_SIZE));
    dot.moveCenter(center);
    dot.moveTop(rect.top() + dotTop);
    painter->fillRect(dot, color);
}

QColor SageIconEngine::messageColor() const
{
    if (m_glyph == SageIconGlyph::Error) {
        return SAGE_COLOR_ERROR;
    }
    if (m_glyph == SageIconGlyph::Warning) {
        return SAGE_COLOR_WARNING;
    }
    return SAGE_COLOR_PRIMARY;
}

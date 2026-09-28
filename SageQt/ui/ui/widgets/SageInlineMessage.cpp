#include "ui/widgets/SageInlineMessage.h"

#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageFontCatalog.h"

#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QSizeF>
#include <QSizePolicy>

SageInlineMessage::SageInlineMessage(SageInlineMessageVariant variant, QWidget* parent)
    : QWidget(parent)
    , m_variant(variant)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

SageInlineMessage::SageInlineMessageVariant SageInlineMessage::variant() const
{
    return m_variant;
}

QString SageInlineMessage::message() const
{
    return m_message;
}

void SageInlineMessage::setMessage(const QString& message)
{
    m_message = message;
    update();
}

void SageInlineMessage::clearMessage()
{
    setMessage(QString());
}

QSize SageInlineMessage::sizeHint() const
{
    return {QWidget::sizeHint().width(), SAGE_INLINE_MSG_HEIGHT};
}

QSize SageInlineMessage::minimumSizeHint() const
{
    return {QWidget::minimumSizeHint().width(), SAGE_INLINE_MSG_HEIGHT};
}

void SageInlineMessage::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    if (m_message.isEmpty()) {
        return;
    }
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect iconRect(QPoint(), QSize(SAGE_INLINE_MSG_ICON_SIZE, SAGE_INLINE_MSG_ICON_SIZE));
    iconRect.moveCenter(QPoint(iconRect.center().x(), rect().center().y()));
    const QPointF iconCenter = QRectF(iconRect).center();
    painter.setPen(QPen(SAGE_COLOR_ERROR, SAGE_BORDER_THICKNESS));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(iconCenter, SAGE_INLINE_ICON_RADIUS, SAGE_INLINE_ICON_RADIUS);
    painter.drawLine(QPointF(iconCenter.x(), iconRect.top() + SAGE_INLINE_ICON_STEM_TOP),
                     QPointF(iconCenter.x(), iconRect.top() + SAGE_INLINE_ICON_STEM_BOTTOM));
    QRectF dot(QPointF(), QSizeF(SAGE_INLINE_ICON_DOT_SIZE, SAGE_INLINE_ICON_DOT_SIZE));
    dot.moveCenter(iconCenter);
    dot.moveTop(iconRect.top() + SAGE_INLINE_ICON_DOT_TOP);
    painter.fillRect(dot, SAGE_COLOR_ERROR);

    const QFont font = SageFontCatalog::font(SageFontRole::Caption);
    const QRect textRect = rect().adjusted(iconRect.right() + SAGE_INLINE_MSG_ICON_GAP, 0, 0, 0);
    painter.setFont(font);
    painter.setPen(SAGE_COLOR_INLINE_ERROR_TEXT);
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter,
                     QFontMetrics(font).elidedText(m_message, Qt::ElideRight, textRect.width()));
}

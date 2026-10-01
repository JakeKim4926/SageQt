#include "ui/widgets/SageSelectionCountLabel.h"

#include "SageDefine.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageFontCatalog.h"

#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QRect>
#include <QSizePolicy>

SageSelectionCountLabel::SageSelectionCountLabel(QWidget* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
}

void SageSelectionCountLabel::setCounts(int totalCount, int selectedCount)
{
    m_totalCount = totalCount;
    m_selectedCount = selectedCount;
    updateGeometry();
    update();
}

QSize SageSelectionCountLabel::sizeHint() const
{
    const QFontMetrics bodyMetrics(SageFontCatalog::font(SageFontRole::Body));
    const QFontMetrics strongMetrics(SageFontCatalog::font(SageFontRole::BodyStrong));
    const int width = bodyMetrics.horizontalAdvance(totalText()) + SAGE_ICON_TEXT_GAP +
                      strongMetrics.horizontalAdvance(selectedText()) + SAGE_ICON_TEXT_GAP +
                      bodyMetrics.horizontalAdvance(SAGE_UI_SELECTION_SUFFIX);
    return {width, SAGE_BUTTON_HEIGHT};
}

QSize SageSelectionCountLabel::minimumSizeHint() const
{
    return sizeHint();
}

void SageSelectionCountLabel::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    const QFont bodyFont = SageFontCatalog::font(SageFontRole::Body);
    const QFont strongFont = SageFontCatalog::font(SageFontRole::BodyStrong);
    int left = 0;

    painter.setFont(bodyFont);
    painter.setPen(SAGE_COLOR_SECONDARY_TEXT);
    painter.drawText(QRect(left, 0, width(), height()), Qt::AlignLeft | Qt::AlignVCenter, totalText());
    left += QFontMetrics(bodyFont).horizontalAdvance(totalText()) + SAGE_ICON_TEXT_GAP;

    painter.setFont(strongFont);
    painter.setPen(SAGE_COLOR_PRIMARY);
    painter.drawText(QRect(left, 0, width() - left, height()), Qt::AlignLeft | Qt::AlignVCenter, selectedText());
    left += QFontMetrics(strongFont).horizontalAdvance(selectedText()) + SAGE_ICON_TEXT_GAP;

    painter.setFont(bodyFont);
    painter.setPen(SAGE_COLOR_SECONDARY_TEXT);
    painter.drawText(QRect(left, 0, width() - left, height()), Qt::AlignLeft | Qt::AlignVCenter,
                     SAGE_UI_SELECTION_SUFFIX);
}

QString SageSelectionCountLabel::totalText() const
{
    return SAGE_UI_SELECTION_TOTAL_FORMAT.arg(m_totalCount);
}

QString SageSelectionCountLabel::selectedText() const
{
    return SAGE_UI_SELECTION_SELECTED_FORMAT.arg(m_selectedCount);
}

#include "ui/style/SageStyle.h"

#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageFontCatalog.h"
#include "ui/style/SageIconEngine.h"
#include "ui/style/SageStyleDefine.h"
#include "ui/widgets/SageButton.h"
#include "ui/widgets/SageLabel.h"
#include "ui/widgets/SageLineEdit.h"
#include "ui/widgets/SageSurface.h"

#include <QAbstractButton>
#include <QBrush>
#include <QComboBox>
#include <QFontMetrics>
#include <QFrame>
#include <QPainter>
#include <QPointF>
#include <QPolygonF>
#include <QRect>
#include <QStyleFactory>
#include <QStyleOption>
#include <QTabBar>
#include <QWidget>
#include <qdrawutil.h>

#include <iterator>

SageStyle::SageStyle()
    : QProxyStyle(QStyleFactory::create(SAGE_STYLE_BASE_NAME))
{
}

QPalette SageStyle::standardPalette() const
{
    QPalette palette = QProxyStyle::standardPalette();
    palette.setColor(QPalette::Window, SAGE_COLOR_APP_BACKGROUND);
    palette.setColor(QPalette::Base, SAGE_COLOR_PANEL);
    palette.setColor(QPalette::AlternateBase, SAGE_COLOR_LIST_ROW_ALT);
    palette.setColor(QPalette::WindowText, SAGE_COLOR_TEXT);
    palette.setColor(QPalette::Text, SAGE_COLOR_TEXT);
    palette.setColor(QPalette::ButtonText, SAGE_COLOR_TEXT);
    palette.setColor(QPalette::PlaceholderText, SAGE_COLOR_TEXT_PLACEHOLDER);
    palette.setColor(QPalette::Button, SAGE_COLOR_PANEL);
    palette.setColor(QPalette::Highlight, SAGE_COLOR_LIST_ROW_SELECTED);
    palette.setColor(QPalette::HighlightedText, SAGE_COLOR_TEXT);
    palette.setColor(QPalette::Accent, SAGE_COLOR_PRIMARY);
    palette.setColor(QPalette::Mid, SAGE_COLOR_BORDER);
    palette.setColor(QPalette::Disabled, QPalette::Text, SAGE_COLOR_TEXT_PLACEHOLDER);
    return palette;
}

void SageStyle::polish(QWidget* widget)
{
    QProxyStyle::polish(widget);
    if (qobject_cast<QTabBar*>(widget) != nullptr) {
        widget->setAttribute(Qt::WA_Hover);
    }
    polishSurface(widget);
    polishLabel(widget);
}

void SageStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
                              const QWidget* widget) const
{
    switch (element) {
    case PE_PanelButtonCommand:
        drawPushButtonPanel(option, painter, widget);
        return;
    case PE_PanelButtonTool:
        drawToolButtonPanel(option, painter);
        return;
    case PE_PanelLineEdit:
        drawLineEditPanel(option, painter, widget);
        return;
    case PE_FrameLineEdit:
        drawLineEditFrame(option, painter, widget);
        return;
    case PE_FrameTabBarBase:
        return;
    case PE_IndicatorCheckBox:
        drawCheckIndicator(option, painter);
        return;
    case PE_PanelStatusBar:
        painter->fillRect(option->rect, SAGE_COLOR_APP_BACKGROUND);
        painter->fillRect(QRect(option->rect.left(), option->rect.top(), option->rect.width(), SAGE_BORDER_THICKNESS),
                          SAGE_COLOR_BORDER);
        return;
    case PE_FrameStatusBarItem:
        return;
    case PE_FrameFocusRect:
        if (qobject_cast<const QAbstractButton*>(widget) != nullptr ||
            qobject_cast<const QTabBar*>(widget) != nullptr) {
            return;
        }
        break;
    default:
        break;
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void SageStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
                            const QWidget* widget) const
{
    if (element == CE_PushButtonLabel) {
        const QStyleOptionButton* buttonOption = qstyleoption_cast<const QStyleOptionButton*>(option);
        if (buttonOption != nullptr && !buttonOption->icon.isNull()) {
            drawPushButtonIconLabel(*buttonOption, painter, pushButtonTextColor(option, widget));
            return;
        }
        if (buttonOption != nullptr) {
            QStyleOptionButton labelOption(*buttonOption);
            labelOption.palette.setColor(QPalette::ButtonText, pushButtonTextColor(option, widget));
            QProxyStyle::drawControl(element, &labelOption, painter, widget);
            return;
        }
    }
    if (element == CE_TabBarTabShape) {
        drawTabShape(option, painter);
        return;
    }
    if (element == CE_TabBarTabLabel) {
        drawTabLabel(option, painter);
        return;
    }
    if (element == CE_MenuItem && qobject_cast<const QComboBox*>(widget) != nullptr) {
        drawComboMenuItem(option, painter, widget);
        return;
    }
    if (element == CE_ComboBoxLabel) {
        const QStyleOptionComboBox* comboOption = qstyleoption_cast<const QStyleOptionComboBox*>(option);
        if (comboOption != nullptr) {
            QRect textRect = option->rect;
            textRect.setRight(subControlRect(CC_ComboBox, comboOption, SC_ComboBoxArrow, widget).left() - 1);
            painter->setPen(SAGE_COLOR_TEXT);
            painter->drawText(textRect, Qt::AlignCenter, comboOption->currentText);
            return;
        }
    }
    if (element == CE_HeaderSection || element == CE_HeaderEmptyArea) {
        painter->fillRect(option->rect, SAGE_COLOR_LIST_HEADER);
        return;
    }
    if (element == CE_HeaderLabel) {
        drawHeaderLabel(option, painter);
        return;
    }
    if (element == CE_ProgressBar) {
        drawProgressBar(option, painter);
        return;
    }
    if (element == CE_ShapedFrame) {
        const QStyleOptionFrame* frameOption = qstyleoption_cast<const QStyleOptionFrame*>(option);
        if (frameOption != nullptr && frameOption->frameShape == QFrame::Box) {
            qDrawPlainRect(painter, option->rect, SAGE_COLOR_BORDER, SAGE_BORDER_THICKNESS);
            return;
        }
        if (frameOption != nullptr && frameOption->frameShape == QFrame::HLine) {
            QRect line = option->rect;
            line.setHeight(SAGE_BORDER_THICKNESS);
            painter->fillRect(line, option->palette.color(QPalette::Mid));
            return;
        }
        if (frameOption != nullptr && frameOption->frameShape == QFrame::VLine) {
            QRect line = option->rect;
            line.setWidth(SAGE_BORDER_THICKNESS);
            painter->fillRect(line, option->palette.color(QPalette::Mid));
            return;
        }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

QSize SageStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& contentsSize,
                                  const QWidget* widget) const
{
    QSize size = QProxyStyle::sizeFromContents(type, option, contentsSize, widget);
    if (type == CT_PushButton) {
        size.setHeight(SAGE_BUTTON_HEIGHT);
    }
    if (type == CT_LineEdit) {
        size.setHeight(SAGE_EDIT_HEIGHT);
    }
    if (type == CT_HeaderSection) {
        size.setHeight(SAGE_LIST_HEADER_HEIGHT);
    }
    if (type == CT_MenuItem && qobject_cast<const QComboBox*>(widget) != nullptr) {
        size.setHeight(SAGE_EDIT_HEIGHT - SAGE_EDIT_BORDER_WIDTH * 2 - SAGE_COMBO_FIELD_INSET);
    }
    if (type == CT_TabBarTab) {
        const QTabBar* tabBar = qobject_cast<const QTabBar*>(widget);
        if (tabBar != nullptr) {
            size = QSize(uniformTabWidth(*tabBar), SAGE_TAB_HEIGHT - SAGE_BORDER_THICKNESS);
        }
    }
    return size;
}

QRect SageStyle::subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const
{
    if (element == SE_LineEditContents) {
        const int textPad = option->state.testFlag(State_ReadOnly) ? SAGE_EDIT_TEXT_LEFT_PAD : SAGE_DLG_EDIT_TEXT_PAD_X;
        const int horizontalInset = SAGE_EDIT_BORDER_WIDTH + textPad - SAGE_QT_LINE_EDIT_TEXT_MARGIN;
        return option->rect.adjusted(horizontalInset, SAGE_EDIT_BORDER_WIDTH, -horizontalInset,
                                     -SAGE_EDIT_BORDER_WIDTH);
    }
    return QProxyStyle::subElementRect(element, option, widget);
}

QIcon SageStyle::standardIcon(StandardPixmap standardIcon, const QStyleOption* option, const QWidget* widget) const
{
    switch (standardIcon) {
    case SP_TitleBarCloseButton:
        return QIcon(new SageIconEngine(SageIconGlyph::Close));
    case SP_BrowserReload:
        return QIcon(new SageIconEngine(SageIconGlyph::Reset));
    case SP_MessageBoxInformation:
        return QIcon(new SageIconEngine(SageIconGlyph::Info));
    case SP_MessageBoxWarning:
        return QIcon(new SageIconEngine(SageIconGlyph::Warning));
    case SP_MessageBoxCritical:
        return QIcon(new SageIconEngine(SageIconGlyph::Error));
    default:
        return QProxyStyle::standardIcon(standardIcon, option, widget);
    }
}

void SageStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter,
                                   const QWidget* widget) const
{
    if (control == CC_ComboBox) {
        drawComboBox(option, painter, *this, widget);
        return;
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

int SageStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    if (metric == PM_IndicatorWidth || metric == PM_IndicatorHeight) {
        return SAGE_LIST_CHECK_BOX_SIZE;
    }
    if (metric == PM_CheckBoxLabelSpacing) {
        return SAGE_SELECTION_CHECK_GLYPH_WIDTH - SAGE_LIST_CHECK_BOX_SIZE;
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

QColor SageStyle::surfaceColor(const QStyleOption* option, const QWidget* widget)
{
    for (const QWidget* ancestor = widget != nullptr ? widget->parentWidget() : nullptr; ancestor != nullptr;
         ancestor = ancestor->parentWidget()) {
        if (ancestor->autoFillBackground()) {
            return ancestor->palette().color(ancestor->backgroundRole());
        }
    }
    return option->palette.color(QPalette::Window);
}

bool SageStyle::isGhostButton(const QWidget* widget)
{
    const SageButton* button = qobject_cast<const SageButton*>(widget);
    return button != nullptr && button->variant() == SageButton::SageButtonVariant::Ghost;
}

void SageStyle::drawPushButtonIconLabel(const QStyleOptionButton& option, QPainter* painter, const QColor& textColor)
{
    const QFontMetrics metrics(option.fontMetrics);
    const int groupWidth = SAGE_ICON_SIZE + SAGE_ICON_TEXT_GAP + metrics.horizontalAdvance(option.text);
    const int groupLeft = option.rect.left() + (option.rect.width() - groupWidth) / 2;
    const QRect iconRect(groupLeft, option.rect.top() + (option.rect.height() - SAGE_ICON_SIZE) / 2, SAGE_ICON_SIZE,
                         SAGE_ICON_SIZE);
    const bool isEnabled = option.state.testFlag(State_Enabled);
    option.icon.paint(painter, iconRect, Qt::AlignCenter, isEnabled ? QIcon::Normal : QIcon::Disabled);
    const QRect textRect(iconRect.right() + 1 + SAGE_ICON_TEXT_GAP, option.rect.top(),
                         option.rect.right() - iconRect.right() - SAGE_ICON_TEXT_GAP, option.rect.height());
    painter->setPen(textColor);
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, option.text);
}

void SageStyle::drawCheckIndicator(const QStyleOption* option, QPainter* painter)
{
    QRect box(0, 0, SAGE_LIST_CHECK_BOX_SIZE, SAGE_LIST_CHECK_BOX_SIZE);
    box.moveCenter(option->rect.center());
    if (!option->state.testFlag(State_On)) {
        const QBrush face(SAGE_COLOR_PANEL);
        qDrawPlainRect(painter, box, SAGE_COLOR_BUTTON_BORDER, SAGE_BORDER_THICKNESS, &face);
        return;
    }
    painter->fillRect(box, SAGE_COLOR_PRIMARY);
    const int inset = SAGE_LIST_CHECK_MARK_THICKNESS * 2;
    const QPointF points[] = {
        QPointF(box.left() + inset, box.top() + box.height() / 2),
        QPointF(box.left() + box.width() / 2, box.top() + box.height() - inset),
        QPointF(box.left() + box.width() - inset, box.top() + inset),
    };
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(SAGE_COLOR_PANEL, SAGE_LIST_CHECK_MARK_THICKNESS));
    painter->drawPolyline(points, std::size(points));
    painter->restore();
}

void SageStyle::drawComboBox(const QStyleOptionComplex* option, QPainter* painter, const QStyle& style,
                             const QWidget* widget)
{
    painter->fillRect(option->rect, SAGE_COLOR_APP_BACKGROUND);
    const QPointF center = QRectF(style.subControlRect(CC_ComboBox, option, SC_ComboBoxArrow, widget)).center();
    const QPolygonF arrow({
        center + QPointF(-SAGE_ICON_ARROW_HALF_WIDTH, -SAGE_ICON_ARROW_HALF_HEIGHT),
        center + QPointF(SAGE_ICON_ARROW_HALF_WIDTH, -SAGE_ICON_ARROW_HALF_HEIGHT),
        center + QPointF(0, SAGE_ICON_ARROW_TIP),
    });
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(SAGE_COLOR_PRIMARY);
    painter->drawPolygon(arrow);
    painter->restore();
}

void SageStyle::drawComboMenuItem(const QStyleOption* option, QPainter* painter, const QWidget* widget)
{
    const QStyleOptionMenuItem* itemOption = qstyleoption_cast<const QStyleOptionMenuItem*>(option);
    if (itemOption == nullptr) {
        return;
    }
    const bool isSelected = option->state.testFlag(State_Selected);
    painter->fillRect(option->rect, isSelected ? SAGE_COLOR_PRIMARY : SAGE_COLOR_PANEL);
    painter->setFont(widget->font());
    painter->setPen(isSelected ? SAGE_COLOR_PANEL : SAGE_COLOR_TEXT);
    painter->drawText(option->rect, Qt::AlignCenter, itemOption->text);
}

bool SageStyle::isPrimaryButton(const QWidget* widget)
{
    const SageButton* button = qobject_cast<const SageButton*>(widget);
    return button != nullptr && button->variant() == SageButton::SageButtonVariant::Primary;
}

void SageStyle::drawPushButtonPanel(const QStyleOption* option, QPainter* painter, const QWidget* widget)
{
    const bool isEnabled = option->state.testFlag(State_Enabled);
    const bool isPressed = option->state.testFlag(State_Sunken);

    if (isPrimaryButton(widget)) {
        const QColor face = !isEnabled ? SAGE_COLOR_BORDER : isPressed ? SAGE_COLOR_PRIMARY_PRESS : SAGE_COLOR_PRIMARY;
        painter->fillRect(option->rect, face);
        return;
    }

    if (isGhostButton(widget)) {
        painter->fillRect(option->rect, isEnabled && isPressed ? SAGE_COLOR_LIST_HEADER : surfaceColor(option, widget));
        return;
    }

    const QBrush face(!isEnabled || isPressed ? SAGE_COLOR_APP_BACKGROUND : SAGE_COLOR_PANEL);
    const QColor border = isEnabled ? SAGE_COLOR_BUTTON_BORDER : SAGE_COLOR_BORDER;
    qDrawPlainRect(painter, option->rect, border, SAGE_BORDER_THICKNESS, &face);
}

void SageStyle::drawToolButtonPanel(const QStyleOption* option, QPainter* painter)
{
    const bool isPressed = option->state.testFlag(State_Sunken);
    painter->fillRect(option->rect, isPressed ? SAGE_COLOR_LIST_HEADER : SAGE_COLOR_PANEL);
}

void SageStyle::drawTabShape(const QStyleOption* option, QPainter* painter)
{
    if (!option->state.testFlag(State_Selected)) {
        return;
    }
    const QRect& tabRect = option->rect;
    painter->fillRect(QRect(tabRect.left(), tabRect.top() + tabRect.height() - SAGE_TAB_INDICATOR_HEIGHT,
                            tabRect.width(), SAGE_TAB_INDICATOR_HEIGHT),
                      SAGE_COLOR_PRIMARY);
}

void SageStyle::drawTabLabel(const QStyleOption* option, QPainter* painter)
{
    const QStyleOptionTab* tabOption = qstyleoption_cast<const QStyleOptionTab*>(option);
    if (tabOption == nullptr) {
        return;
    }
    const bool isSelected = option->state.testFlag(State_Selected);
    const QRect textRect = option->rect.adjusted(0, 0, 0, -SAGE_TAB_INDICATOR_HEIGHT);
    painter->save();
    painter->setFont(SageFontCatalog::font(isSelected ? SageFontRole::BodyStrong : SageFontRole::Body));
    const bool isHovered = option->state.testFlag(State_MouseOver);
    painter->setPen(isSelected || isHovered ? SAGE_COLOR_TEXT : SAGE_COLOR_SECONDARY_TEXT);
    painter->drawText(textRect, Qt::AlignCenter, tabOption->text);
    painter->restore();
}

int SageStyle::uniformTabWidth(const QTabBar& tabBar)
{
    const QFontMetrics metrics(SageFontCatalog::font(SageFontRole::BodyStrong));
    int widestText = 0;
    for (int index = 0; index < tabBar.count(); ++index) {
        widestText = qMax(widestText, metrics.horizontalAdvance(tabBar.tabText(index)));
    }
    return widestText + SAGE_TAB_PAD_X + SAGE_TAB_PAD_X;
}

void SageStyle::drawLineEditPanel(const QStyleOption* option, QPainter* painter, const QWidget* widget)
{
    const bool isEnabled = option->state.testFlag(State_Enabled);
    const bool isReadOnly = option->state.testFlag(State_ReadOnly);
    const QColor face = !isEnabled ? SAGE_COLOR_LIST_HEADER : isReadOnly ? SAGE_COLOR_APP_BACKGROUND : SAGE_COLOR_PANEL;
    painter->fillRect(option->rect, face);
    const QStyleOptionFrame* frameOption = qstyleoption_cast<const QStyleOptionFrame*>(option);
    if (frameOption != nullptr && frameOption->lineWidth > 0) {
        drawLineEditFrame(option, painter, widget);
    }
}

void SageStyle::drawLineEditFrame(const QStyleOption* option, QPainter* painter, const QWidget* widget)
{
    const SageLineEdit* lineEdit = qobject_cast<const SageLineEdit*>(widget);
    const bool isError = lineEdit != nullptr && lineEdit->variant() == SageLineEdit::SageLineEditVariant::Error;
    const bool hasFocus = option->state.testFlag(State_HasFocus);
    const QColor border = isError ? SAGE_COLOR_ERROR : hasFocus ? SAGE_COLOR_PRIMARY : SAGE_COLOR_BORDER;
    qDrawPlainRect(painter, option->rect, border, SAGE_EDIT_BORDER_WIDTH);
}

void SageStyle::drawHeaderLabel(const QStyleOption* option, QPainter* painter)
{
    const QStyleOptionHeader* headerOption = qstyleoption_cast<const QStyleOptionHeader*>(option);
    if (headerOption == nullptr) {
        return;
    }
    const QFont font = SageFontCatalog::font(SageFontRole::List);
    painter->setFont(font);
    painter->setPen(SAGE_COLOR_TEXT_MUTED);
    painter->drawText(option->rect, Qt::AlignCenter,
                      QFontMetrics(font).elidedText(headerOption->text, Qt::ElideRight, option->rect.width()));
}

void SageStyle::drawProgressBar(const QStyleOption* option, QPainter* painter)
{
    painter->fillRect(option->rect, SAGE_COLOR_LIST_GRID);
    const QStyleOptionProgressBar* barOption = qstyleoption_cast<const QStyleOptionProgressBar*>(option);
    if (barOption == nullptr || barOption->maximum <= barOption->minimum) {
        return;
    }
    const qint64 range = static_cast<qint64>(barOption->maximum) - barOption->minimum;
    const qint64 progress = static_cast<qint64>(barOption->progress) - barOption->minimum;
    const int fillWidth = static_cast<int>((option->rect.width() * progress + range / 2) / range);
    if (fillWidth <= 0) {
        return;
    }
    QRect fill = option->rect;
    fill.setWidth(fillWidth);
    painter->fillRect(fill, SAGE_COLOR_PRIMARY);
}

void SageStyle::polishSurface(QWidget* widget)
{
    const SageSurface* surface = qobject_cast<const SageSurface*>(widget);
    if (surface == nullptr) {
        return;
    }
    QPalette palette = widget->palette();
    if (surface->variant() == SageSurface::SageSurfaceVariant::Sidebar) {
        palette.setColor(QPalette::Window, SAGE_COLOR_SIDEBAR);
        palette.setColor(QPalette::Base, SAGE_COLOR_SIDEBAR);
        palette.setColor(QPalette::WindowText, SAGE_COLOR_SIDEBAR_TEXT);
        palette.setColor(QPalette::Text, SAGE_COLOR_SIDEBAR_TEXT);
        palette.setColor(QPalette::Mid, SAGE_COLOR_SIDEBAR_DIVIDER);
    }
    if (surface->variant() == SageSurface::SageSurfaceVariant::Panel) {
        palette.setColor(QPalette::Window, SAGE_COLOR_PANEL);
    }
    widget->setPalette(palette);
    widget->setAutoFillBackground(true);
}

void SageStyle::polishLabel(QWidget* widget)
{
    const SageLabel* label = qobject_cast<const SageLabel*>(widget);
    if (label == nullptr) {
        return;
    }
    switch (label->variant()) {
    case SageLabel::SageLabelVariant::Title:
        widget->setFont(SageFontCatalog::font(SageFontRole::Title));
        return;
    case SageLabel::SageLabelVariant::SecondaryCaption:
        widget->setFont(SageFontCatalog::font(SageFontRole::Caption));
        setTextColor(widget, SAGE_COLOR_SECONDARY_TEXT);
        return;
    case SageLabel::SageLabelVariant::Hint:
        widget->setFont(SageFontCatalog::font(SageFontRole::Body));
        setTextColor(widget, SAGE_COLOR_SECONDARY_TEXT);
        return;
    case SageLabel::SageLabelVariant::MutedCaption:
        widget->setFont(SageFontCatalog::font(SageFontRole::Caption));
        setTextColor(widget, SAGE_COLOR_TEXT_MUTED);
        return;
    case SageLabel::SageLabelVariant::Section:
    case SageLabel::SageLabelVariant::TableTitle: {
        widget->setFont(SageFontCatalog::font(
            label->variant() == SageLabel::SageLabelVariant::Section ? SageFontRole::Section : SageFontRole::Body));
        QPalette palette = widget->palette();
        palette.setColor(QPalette::Window, SAGE_COLOR_LIST_HEADER);
        widget->setPalette(palette);
        widget->setBackgroundRole(QPalette::Window);
        widget->setAutoFillBackground(true);
        return;
    }
    case SageLabel::SageLabelVariant::FormLabel:
        widget->setFont(SageFontCatalog::font(SageFontRole::Body));
        setTextColor(widget, SAGE_COLOR_TEXT_MUTED);
        return;
    case SageLabel::SageLabelVariant::SidebarLogo:
        widget->setFont(SageFontCatalog::font(SageFontRole::Logo));
        return;
    }
}

void SageStyle::setTextColor(QWidget* widget, const QColor& color)
{
    QPalette palette = widget->palette();
    palette.setColor(QPalette::WindowText, color);
    widget->setPalette(palette);
}

QColor SageStyle::pushButtonTextColor(const QStyleOption* option, const QWidget* widget)
{
    if (isGhostButton(widget)) {
        return option->state.testFlag(State_Enabled) ? SAGE_COLOR_TEXT_MUTED : SAGE_COLOR_BORDER;
    }
    if (!option->state.testFlag(State_Enabled)) {
        return SAGE_COLOR_SECONDARY_TEXT;
    }
    return isPrimaryButton(widget) ? SAGE_COLOR_BUTTON_TEXT : SAGE_COLOR_TEXT;
}

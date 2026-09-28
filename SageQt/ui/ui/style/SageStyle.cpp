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
#include <QFrame>
#include <QPainter>
#include <QRect>
#include <QStyleFactory>
#include <QStyleOption>
#include <QWidget>
#include <qdrawutil.h>

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
    case PE_FrameFocusRect:
        if (qobject_cast<const QAbstractButton*>(widget) != nullptr) {
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
        if (buttonOption != nullptr) {
            QStyleOptionButton labelOption(*buttonOption);
            labelOption.palette.setColor(QPalette::ButtonText, pushButtonTextColor(option, widget));
            QProxyStyle::drawControl(element, &labelOption, painter, widget);
            return;
        }
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
    return size;
}

QRect SageStyle::subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const
{
    if (element == SE_LineEditContents) {
        const int horizontalInset = SAGE_EDIT_BORDER_WIDTH + SAGE_DLG_EDIT_TEXT_PAD_X - SAGE_QT_LINE_EDIT_TEXT_MARGIN;
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

    const QBrush face(!isEnabled || isPressed ? SAGE_COLOR_APP_BACKGROUND : SAGE_COLOR_PANEL);
    const QColor border = isEnabled ? SAGE_COLOR_BUTTON_BORDER : SAGE_COLOR_BORDER;
    qDrawPlainRect(painter, option->rect, border, SAGE_BORDER_THICKNESS, &face);
}

void SageStyle::drawToolButtonPanel(const QStyleOption* option, QPainter* painter)
{
    const bool isPressed = option->state.testFlag(State_Sunken);
    painter->fillRect(option->rect, isPressed ? SAGE_COLOR_LIST_HEADER : SAGE_COLOR_PANEL);
}

void SageStyle::drawLineEditPanel(const QStyleOption* option, QPainter* painter, const QWidget* widget)
{
    const bool isEnabled = option->state.testFlag(State_Enabled);
    painter->fillRect(option->rect, isEnabled ? SAGE_COLOR_PANEL : SAGE_COLOR_LIST_HEADER);
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
    if (surface->variant() == SageSurface::SageSurfaceVariant::Header) {
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
    case SageLabel::SageLabelVariant::MutedCaption:
        widget->setFont(SageFontCatalog::font(SageFontRole::Caption));
        setTextColor(widget, SAGE_COLOR_TEXT_MUTED);
        return;
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
    if (!option->state.testFlag(State_Enabled)) {
        return SAGE_COLOR_SECONDARY_TEXT;
    }
    return isPrimaryButton(widget) ? SAGE_COLOR_BUTTON_TEXT : SAGE_COLOR_TEXT;
}

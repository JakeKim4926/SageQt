#pragma once

#include <QColor>
#include <QIcon>
#include <QPalette>
#include <QProxyStyle>
#include <QRect>
#include <QSize>

class QPainter;
class QStyleOption;
class QTabBar;
class QWidget;

class SageStyle : public QProxyStyle
{
    Q_OBJECT

public:
    SageStyle();

    QPalette standardPalette() const override;
    void polish(QWidget* widget) override;
    using QProxyStyle::polish;
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
                       const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
                     const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& contentsSize,
                           const QWidget* widget = nullptr) const override;
    QRect subElementRect(SubElement element, const QStyleOption* option,
                         const QWidget* widget = nullptr) const override;
    QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption* option = nullptr,
                       const QWidget* widget = nullptr) const override;

private:
    static bool isPrimaryButton(const QWidget* widget);
    static void drawPushButtonPanel(const QStyleOption* option, QPainter* painter, const QWidget* widget);
    static void drawToolButtonPanel(const QStyleOption* option, QPainter* painter);
    static void drawTabShape(const QStyleOption* option, QPainter* painter);
    static void drawTabLabel(const QStyleOption* option, QPainter* painter);
    static int uniformTabWidth(const QTabBar& tabBar);
    static void drawLineEditPanel(const QStyleOption* option, QPainter* painter, const QWidget* widget);
    static void drawLineEditFrame(const QStyleOption* option, QPainter* painter, const QWidget* widget);
    static QColor pushButtonTextColor(const QStyleOption* option, const QWidget* widget);
    static void polishSurface(QWidget* widget);
    static void polishLabel(QWidget* widget);
    static void setTextColor(QWidget* widget, const QColor& color);
};

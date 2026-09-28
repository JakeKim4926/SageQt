#pragma once

#include <QColor>
#include <QIcon>
#include <QPalette>
#include <QProxyStyle>
#include <QSize>

class QPainter;
class QStyleOption;
class QWidget;

class SageStyle : public QProxyStyle
{
    Q_OBJECT

public:
    SageStyle();

    QPalette standardPalette() const override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
                       const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
                     const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& contentsSize,
                           const QWidget* widget = nullptr) const override;
    QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption* option = nullptr,
                       const QWidget* widget = nullptr) const override;

private:
    static bool isPrimaryButton(const QWidget* widget);
    static void drawPushButtonPanel(const QStyleOption* option, QPainter* painter, const QWidget* widget);
    static void drawToolButtonPanel(const QStyleOption* option, QPainter* painter);
    static QColor pushButtonTextColor(const QStyleOption* option, const QWidget* widget);
};

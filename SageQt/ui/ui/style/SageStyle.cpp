#include "ui/style/SageStyle.h"

#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageStyleDefine.h"

#include <QStyleFactory>

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
    return palette;
}

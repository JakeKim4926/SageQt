#pragma once

#include <QPalette>
#include <QProxyStyle>

class SageStyle : public QProxyStyle
{
    Q_OBJECT

public:
    SageStyle();

    QPalette standardPalette() const override;
};

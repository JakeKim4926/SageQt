#pragma once

#include <QFont>

enum class SageFontRole
{
    Body,
    BodyStrong,
    Title,
    Section,
    List,
    Caption,
    Summary,
    Logo
};

class SageFontCatalog
{
public:
    static QFont font(SageFontRole role);
    static int pixelSize(int pointSizeTenths);

private:
    static QFont makeFont(const QString& family, QFont::Weight weight, int pointSizeTenths);
};

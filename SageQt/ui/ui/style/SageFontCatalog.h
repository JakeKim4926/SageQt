#pragma once

#include <QFont>
#include <QStringView>

enum class SageFontRole
{
    Body,
    BodyStrong,
    Title,
    Section,
    List,
    ListBold,
    ListStrong,
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
    static QFont makeFont(QStringView family, QFont::Weight weight, int pointSizeTenths);
};

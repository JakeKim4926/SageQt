#include "ui/style/SageFontCatalog.h"

#include "ui/style/SageDesignDefine.h"

#include <QtMath>

QFont SageFontCatalog::font(SageFontRole role)
{
    switch (role) {
    case SageFontRole::Body:
        return makeFont(SAGE_FONT_FAMILY_PRETENDARD, QFont::Normal, SAGE_CONTROL_FONT_POINT_SIZE);
    case SageFontRole::Title:
        return makeFont(SAGE_FONT_FAMILY_PRETENDARD, QFont::DemiBold, SAGE_TITLE_FONT_POINT_SIZE);
    case SageFontRole::Section:
        return makeFont(SAGE_FONT_FAMILY_PRETENDARD, QFont::DemiBold, SAGE_HEADER_FONT_POINT_SIZE);
    case SageFontRole::List:
        return makeFont(SAGE_FONT_FAMILY_PRETENDARD, QFont::Normal, SAGE_LIST_FONT_POINT_SIZE);
    case SageFontRole::Caption:
        return makeFont(SAGE_FONT_FAMILY_PRETENDARD, QFont::Normal, SAGE_CAPTION_FONT_POINT_SIZE);
    case SageFontRole::Summary:
        return makeFont(SAGE_FONT_FAMILY_PRETENDARD, QFont::DemiBold, SAGE_SUMMARY_FONT_POINT_SIZE);
    case SageFontRole::Logo:
        return makeFont(SAGE_FONT_FAMILY_GMARKET, QFont::Bold, SAGE_TITLE_FONT_POINT_SIZE);
    }
    return makeFont(SAGE_FONT_FAMILY_PRETENDARD, QFont::Normal, SAGE_CONTROL_FONT_POINT_SIZE);
}

int SageFontCatalog::pixelSize(int pointSizeTenths)
{
    return qRound(pointSizeTenths / SAGE_FONT_TENTHS_PER_POINT * SAGE_FONT_REFERENCE_DPI / SAGE_FONT_POINTS_PER_INCH);
}

QFont SageFontCatalog::makeFont(const QString& family, QFont::Weight weight, int pointSizeTenths)
{
    QFont font(family);
    font.setWeight(weight);
    font.setPixelSize(pixelSize(pointSizeTenths));
    return font;
}

#pragma once

#include "ui/style/SageDesignDefine.h"

#include <QList>
#include <QString>

struct SageFontFile
{
    QString m_resourcePath;
    QString m_family;
};

inline const QString SAGE_STYLE_BASE_NAME = QStringLiteral("Fusion");
inline constexpr int SAGE_FONT_INVALID_ID = -1;
inline constexpr double SAGE_ROUND_RECT_DIAMETER_TO_RADIUS = 0.5;
inline constexpr double SAGE_STROKE_CENTER_OFFSET = 0.5;
inline constexpr double SAGE_FULL_CIRCLE_DEGREES = 360.0;
inline constexpr int SAGE_QT_ARC_UNITS_PER_DEGREE = 16;
inline constexpr int SAGE_QT_LINE_EDIT_TEXT_MARGIN = 2;
inline constexpr int SAGE_QT_STATUS_BAR_ITEM_OFFSET = 2;

inline const QList<SageFontFile> SAGE_FONT_FILES = {
    {QStringLiteral(":/fonts/PretendardRegular.ttf"), SAGE_FONT_FAMILY_PRETENDARD.toString()},
    {QStringLiteral(":/fonts/PretendardSemiBold.ttf"), SAGE_FONT_FAMILY_PRETENDARD.toString()},
    {QStringLiteral(":/fonts/PretendardBold.ttf"), SAGE_FONT_FAMILY_PRETENDARD.toString()},
    {QStringLiteral(":/fonts/GmarketSansTTFBold.ttf"), SAGE_FONT_FAMILY_GMARKET.toString()},
};

inline const QString SAGE_FONT_ERROR_REGISTER = QStringLiteral("폰트를 등록할 수 없습니다. Path=%1");
inline const QString SAGE_FONT_ERROR_FAMILY = QStringLiteral("폰트 패밀리가 다릅니다. Path=%1, Expected=%2, Actual=%3");
inline const QString SAGE_FONT_FAMILY_SEPARATOR = QStringLiteral(", ");

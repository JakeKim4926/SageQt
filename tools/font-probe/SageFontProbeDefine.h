#pragma once

#include <QFont>
#include <QList>
#include <QString>
#include <QStringList>

struct SageFontProbeRole
{
    QString m_name;
    QString m_family;
    QFont::Weight m_weight;
    int m_pointSizeTenths;
};

inline constexpr double SAGE_FONT_PROBE_TENTHS_PER_POINT = 10.0;
inline constexpr double SAGE_FONT_PROBE_REFERENCE_DPI = 96.0;
inline constexpr double SAGE_FONT_PROBE_POINTS_PER_INCH = 72.0;
inline constexpr int SAGE_FONT_PROBE_INVALID_FONT_ID = -1;
inline constexpr qsizetype SAGE_FONT_PROBE_ARGUMENT_COUNT = 3;
inline constexpr qsizetype SAGE_FONT_PROBE_FONT_DIRECTORY_ARGUMENT = 1;
inline constexpr qsizetype SAGE_FONT_PROBE_OUTPUT_FILE_ARGUMENT = 2;

inline const QString SAGE_FONT_PROBE_USAGE = QStringLiteral("usage: SageFontProbe <font-directory> <output-json>");
inline const QString SAGE_FONT_PROBE_ERROR_REGISTER = QStringLiteral("failed to register font: %1");
inline const QString SAGE_FONT_PROBE_ERROR_NO_SCREEN = QStringLiteral("no primary screen");
inline const QString SAGE_FONT_PROBE_ERROR_OPEN_OUTPUT = QStringLiteral("failed to open output file: %1");
inline const QString SAGE_FONT_PROBE_ERROR_WRITE_OUTPUT = QStringLiteral("failed to write output file: %1");

inline const QStringList SAGE_FONT_PROBE_FONT_FILES = {
    QStringLiteral("PretendardRegular.ttf"),   QStringLiteral("PretendardSemiBold.ttf"),
    QStringLiteral("PretendardBold.ttf"),      QStringLiteral("GmarketSansTTFBold.ttf"),
    QStringLiteral("GmarketSansTTFLight.ttf"), QStringLiteral("GmarketSansTTFMedium.ttf"),
};

inline const QStringList SAGE_FONT_PROBE_LEGACY_FACES = {
    QStringLiteral("Pretendard SemiBold"),
    QStringLiteral("Pretendard"),
    QStringLiteral("Gmarket Sans TTF Bold"),
};

inline const QString SAGE_FONT_PROBE_FAMILY_PRETENDARD = QStringLiteral("Pretendard");
inline const QString SAGE_FONT_PROBE_FAMILY_GMARKET = QStringLiteral("Gmarket Sans TTF");

inline const QList<SageFontProbeRole> SAGE_FONT_PROBE_ROLES = {
    {QStringLiteral("control"), SAGE_FONT_PROBE_FAMILY_PRETENDARD, QFont::Normal, 105},
    {QStringLiteral("content"), SAGE_FONT_PROBE_FAMILY_PRETENDARD, QFont::Normal, 105},
    {QStringLiteral("title"), SAGE_FONT_PROBE_FAMILY_PRETENDARD, QFont::DemiBold, 143},
    {QStringLiteral("header"), SAGE_FONT_PROBE_FAMILY_PRETENDARD, QFont::DemiBold, 113},
    {QStringLiteral("contentSemiBold"), SAGE_FONT_PROBE_FAMILY_PRETENDARD, QFont::DemiBold, 105},
    {QStringLiteral("caption"), SAGE_FONT_PROBE_FAMILY_PRETENDARD, QFont::Normal, 90},
    {QStringLiteral("summary"), SAGE_FONT_PROBE_FAMILY_PRETENDARD, QFont::DemiBold, 128},
    {QStringLiteral("list"), SAGE_FONT_PROBE_FAMILY_PRETENDARD, QFont::Normal, 98},
    {QStringLiteral("listSemiBold"), SAGE_FONT_PROBE_FAMILY_PRETENDARD, QFont::DemiBold, 98},
    {QStringLiteral("listBold"), SAGE_FONT_PROBE_FAMILY_PRETENDARD, QFont::Bold, 98},
    {QStringLiteral("logo"), SAGE_FONT_PROBE_FAMILY_GMARKET, QFont::Bold, 143},
};

inline const QStringList SAGE_FONT_PROBE_SAMPLE_TEXTS = {
    QStringLiteral("샘플 업무"),
    QStringLiteral("실행 기록"),
    QStringLiteral("입력 파일"),
    QStringLiteral("검색어 입력"),
    QStringLiteral("초기화"),
    QStringLiteral("샘플 업무가 완료되었습니다."),
    QStringLiteral("Result"),
    QStringLiteral("1,234,567"),
    QStringLiteral("The Quick Brown Fox Jumps Over The Lazy Dog"),
};

inline const QString SAGE_FONT_PROBE_KEY_ENVIRONMENT = QStringLiteral("environment");
inline const QString SAGE_FONT_PROBE_KEY_QT_VERSION = QStringLiteral("qtVersion");
inline const QString SAGE_FONT_PROBE_KEY_PLATFORM = QStringLiteral("platform");
inline const QString SAGE_FONT_PROBE_KEY_OS = QStringLiteral("os");
inline const QString SAGE_FONT_PROBE_KEY_LOGICAL_DPI = QStringLiteral("logicalDpi");
inline const QString SAGE_FONT_PROBE_KEY_PHYSICAL_DPI = QStringLiteral("physicalDpi");
inline const QString SAGE_FONT_PROBE_KEY_DEVICE_PIXEL_RATIO = QStringLiteral("devicePixelRatio");
inline const QString SAGE_FONT_PROBE_KEY_REGISTERED_FONTS = QStringLiteral("registeredFonts");
inline const QString SAGE_FONT_PROBE_KEY_FILE = QStringLiteral("file");
inline const QString SAGE_FONT_PROBE_KEY_FAMILIES = QStringLiteral("families");
inline const QString SAGE_FONT_PROBE_KEY_FAMILY = QStringLiteral("family");
inline const QString SAGE_FONT_PROBE_KEY_STYLES = QStringLiteral("styles");
inline const QString SAGE_FONT_PROBE_KEY_LEGACY_FACES = QStringLiteral("legacyFaces");
inline const QString SAGE_FONT_PROBE_KEY_FACE = QStringLiteral("face");
inline const QString SAGE_FONT_PROBE_KEY_ROLES = QStringLiteral("roles");
inline const QString SAGE_FONT_PROBE_KEY_ROLE = QStringLiteral("role");
inline const QString SAGE_FONT_PROBE_KEY_REQUESTED_FAMILY = QStringLiteral("requestedFamily");
inline const QString SAGE_FONT_PROBE_KEY_REQUESTED_WEIGHT = QStringLiteral("requestedWeight");
inline const QString SAGE_FONT_PROBE_KEY_POINT_SIZE = QStringLiteral("pointSize");
inline const QString SAGE_FONT_PROBE_KEY_PIXEL_SIZE = QStringLiteral("pixelSize");
inline const QString SAGE_FONT_PROBE_KEY_PIXEL_METRICS = QStringLiteral("pixelMetrics");
inline const QString SAGE_FONT_PROBE_KEY_RESOLVED_FAMILY = QStringLiteral("resolvedFamily");
inline const QString SAGE_FONT_PROBE_KEY_RESOLVED_STYLE = QStringLiteral("resolvedStyle");
inline const QString SAGE_FONT_PROBE_KEY_RESOLVED_WEIGHT = QStringLiteral("resolvedWeight");
inline const QString SAGE_FONT_PROBE_KEY_EXACT_MATCH = QStringLiteral("exactMatch");
inline const QString SAGE_FONT_PROBE_KEY_HEIGHT = QStringLiteral("height");
inline const QString SAGE_FONT_PROBE_KEY_ASCENT = QStringLiteral("ascent");
inline const QString SAGE_FONT_PROBE_KEY_DESCENT = QStringLiteral("descent");
inline const QString SAGE_FONT_PROBE_KEY_LINE_SPACING = QStringLiteral("lineSpacing");
inline const QString SAGE_FONT_PROBE_KEY_TEXTS = QStringLiteral("texts");
inline const QString SAGE_FONT_PROBE_KEY_TEXT = QStringLiteral("text");
inline const QString SAGE_FONT_PROBE_KEY_ADVANCE = QStringLiteral("advance");
inline const QString SAGE_FONT_PROBE_KEY_BOUNDING_WIDTH = QStringLiteral("boundingWidth");
inline const QString SAGE_FONT_PROBE_KEY_BOUNDING_HEIGHT = QStringLiteral("boundingHeight");

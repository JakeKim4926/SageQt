#include "SageFontProbe.h"

#include "SageFontProbeDefine.h"

#include <QDir>
#include <QFontDatabase>
#include <QFontInfo>
#include <QFontMetricsF>
#include <QGuiApplication>
#include <QRectF>
#include <QScreen>
#include <QStringList>
#include <QSysInfo>

bool SageFontProbe::run(const QString& fontDirectory, QJsonObject& outReport, QString& outError) const
{
    QJsonObject environment;
    if (!describeEnvironment(environment, outError)) {
        return false;
    }

    QJsonArray registeredFonts;
    if (!registerFonts(fontDirectory, registeredFonts, outError)) {
        return false;
    }

    outReport.insert(SAGE_FONT_PROBE_KEY_ENVIRONMENT, environment);
    outReport.insert(SAGE_FONT_PROBE_KEY_REGISTERED_FONTS, registeredFonts);
    outReport.insert(SAGE_FONT_PROBE_KEY_LEGACY_FACES, describeLegacyFaces());
    outReport.insert(SAGE_FONT_PROBE_KEY_ROLES, describeRoles());
    return true;
}

bool SageFontProbe::registerFonts(const QString& fontDirectory, QJsonArray& outRegisteredFonts, QString& outError)
{
    const QDir directory(fontDirectory);
    for (const QString& fileName : SAGE_FONT_PROBE_FONT_FILES) {
        const int fontId = QFontDatabase::addApplicationFont(directory.absoluteFilePath(fileName));
        if (fontId == SAGE_FONT_PROBE_INVALID_FONT_ID) {
            outError = SAGE_FONT_PROBE_ERROR_REGISTER.arg(fileName);
            return false;
        }

        QJsonArray families;
        const QStringList familyNames = QFontDatabase::applicationFontFamilies(fontId);
        for (const QString& familyName : familyNames) {
            QJsonObject family;
            family.insert(SAGE_FONT_PROBE_KEY_FAMILY, familyName);
            family.insert(SAGE_FONT_PROBE_KEY_STYLES, QJsonArray::fromStringList(QFontDatabase::styles(familyName)));
            families.append(family);
        }

        QJsonObject registeredFont;
        registeredFont.insert(SAGE_FONT_PROBE_KEY_FILE, fileName);
        registeredFont.insert(SAGE_FONT_PROBE_KEY_FAMILIES, families);
        outRegisteredFonts.append(registeredFont);
    }
    return true;
}

bool SageFontProbe::describeEnvironment(QJsonObject& outEnvironment, QString& outError)
{
    const QScreen* screen = QGuiApplication::primaryScreen();
    if (screen == nullptr) {
        outError = SAGE_FONT_PROBE_ERROR_NO_SCREEN;
        return false;
    }

    outEnvironment.insert(SAGE_FONT_PROBE_KEY_QT_VERSION, QString::fromLatin1(qVersion()));
    outEnvironment.insert(SAGE_FONT_PROBE_KEY_PLATFORM, QGuiApplication::platformName());
    outEnvironment.insert(SAGE_FONT_PROBE_KEY_OS, QSysInfo::prettyProductName());
    outEnvironment.insert(SAGE_FONT_PROBE_KEY_LOGICAL_DPI, screen->logicalDotsPerInch());
    outEnvironment.insert(SAGE_FONT_PROBE_KEY_PHYSICAL_DPI, screen->physicalDotsPerInch());
    outEnvironment.insert(SAGE_FONT_PROBE_KEY_DEVICE_PIXEL_RATIO, screen->devicePixelRatio());
    return true;
}

QJsonArray SageFontProbe::describeLegacyFaces()
{
    QJsonArray legacyFaces;
    for (const QString& face : SAGE_FONT_PROBE_LEGACY_FACES) {
        QJsonObject legacyFace = describeResolvedFont(QFont(face));
        legacyFace.insert(SAGE_FONT_PROBE_KEY_FACE, face);
        legacyFaces.append(legacyFace);
    }
    return legacyFaces;
}

QJsonArray SageFontProbe::describeRoles()
{
    QJsonArray roles;
    for (const SageFontProbeRole& role : SAGE_FONT_PROBE_ROLES) {
        QFont font(role.m_family);
        font.setWeight(role.m_weight);
        font.setPointSizeF(role.m_pointSizeTenths / SAGE_FONT_PROBE_TENTHS_PER_POINT);

        const QFontMetricsF metrics(font);
        QJsonObject roleReport = describeResolvedFont(font);
        roleReport.insert(SAGE_FONT_PROBE_KEY_ROLE, role.m_name);
        roleReport.insert(SAGE_FONT_PROBE_KEY_REQUESTED_FAMILY, role.m_family);
        roleReport.insert(SAGE_FONT_PROBE_KEY_REQUESTED_WEIGHT, static_cast<int>(role.m_weight));
        roleReport.insert(SAGE_FONT_PROBE_KEY_POINT_SIZE, font.pointSizeF());
        roleReport.insert(SAGE_FONT_PROBE_KEY_HEIGHT, metrics.height());
        roleReport.insert(SAGE_FONT_PROBE_KEY_ASCENT, metrics.ascent());
        roleReport.insert(SAGE_FONT_PROBE_KEY_DESCENT, metrics.descent());
        roleReport.insert(SAGE_FONT_PROBE_KEY_LINE_SPACING, metrics.lineSpacing());
        roleReport.insert(SAGE_FONT_PROBE_KEY_TEXTS, measureTexts(font));
        roles.append(roleReport);
    }
    return roles;
}

QJsonObject SageFontProbe::describeResolvedFont(const QFont& font)
{
    const QFontInfo info(font);
    QJsonObject resolved;
    resolved.insert(SAGE_FONT_PROBE_KEY_RESOLVED_FAMILY, info.family());
    resolved.insert(SAGE_FONT_PROBE_KEY_RESOLVED_STYLE, info.styleName());
    resolved.insert(SAGE_FONT_PROBE_KEY_RESOLVED_WEIGHT, info.weight());
    resolved.insert(SAGE_FONT_PROBE_KEY_EXACT_MATCH, info.exactMatch());
    return resolved;
}

QJsonArray SageFontProbe::measureTexts(const QFont& font)
{
    const QFontMetricsF metrics(font);
    QJsonArray texts;
    for (const QString& text : SAGE_FONT_PROBE_SAMPLE_TEXTS) {
        const QRectF bounds = metrics.boundingRect(text);
        QJsonObject measured;
        measured.insert(SAGE_FONT_PROBE_KEY_TEXT, text);
        measured.insert(SAGE_FONT_PROBE_KEY_ADVANCE, metrics.horizontalAdvance(text));
        measured.insert(SAGE_FONT_PROBE_KEY_BOUNDING_WIDTH, bounds.width());
        measured.insert(SAGE_FONT_PROBE_KEY_BOUNDING_HEIGHT, bounds.height());
        texts.append(measured);
    }
    return texts;
}

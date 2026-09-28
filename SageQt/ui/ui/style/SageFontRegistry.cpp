#include "ui/style/SageFontRegistry.h"

#include "ui/style/SageStyleDefine.h"

#include <QFontDatabase>
#include <QStringList>

bool SageFontRegistry::registerApplicationFonts(QString& outError)
{
    for (const SageFontFile& file : SAGE_FONT_FILES) {
        const int fontId = QFontDatabase::addApplicationFont(file.m_resourcePath);
        if (fontId == SAGE_FONT_INVALID_ID) {
            outError = SAGE_FONT_ERROR_REGISTER.arg(file.m_resourcePath);
            return false;
        }

        const QStringList families = QFontDatabase::applicationFontFamilies(fontId);
        if (!families.contains(file.m_family)) {
            outError = SAGE_FONT_ERROR_FAMILY.arg(file.m_resourcePath, file.m_family,
                                                  families.join(SAGE_FONT_FAMILY_SEPARATOR));
            return false;
        }
    }
    return true;
}

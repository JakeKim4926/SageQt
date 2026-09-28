#pragma once

#include <QString>

class SageFontRegistry
{
public:
    static bool registerApplicationFonts(QString& outError);
};

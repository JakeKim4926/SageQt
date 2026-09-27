#pragma once

#include <QFont>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>

class SageFontProbe
{
public:
    bool run(const QString& fontDirectory, QJsonObject& outReport, QString& outError) const;

private:
    static bool registerFonts(const QString& fontDirectory, QJsonArray& outRegisteredFonts, QString& outError);
    static bool describeEnvironment(QJsonObject& outEnvironment, QString& outError);
    static QJsonArray describeLegacyFaces();
    static QJsonArray describeRoles();
    static QJsonObject describeResolvedFont(const QFont& font);
    static QJsonArray measureTexts(const QFont& font);
};

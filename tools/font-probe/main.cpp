#include "SageFontProbe.h"
#include "SageFontProbeDefine.h"

#include <QByteArray>
#include <QFile>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QtLogging>

#include <cstdlib>

int main(int argc, char* argv[])
{
    const QGuiApplication application(argc, argv);
    const QStringList arguments = QGuiApplication::arguments();
    if (arguments.size() != SAGE_FONT_PROBE_ARGUMENT_COUNT) {
        qCritical().noquote() << SAGE_FONT_PROBE_USAGE;
        return EXIT_FAILURE;
    }

    const SageFontProbe probe;
    QJsonObject report;
    QString error;
    if (!probe.run(arguments.at(SAGE_FONT_PROBE_FONT_DIRECTORY_ARGUMENT), report, error)) {
        qCritical().noquote() << error;
        return EXIT_FAILURE;
    }

    const QString outputPath = arguments.at(SAGE_FONT_PROBE_OUTPUT_FILE_ARGUMENT);
    QFile output(outputPath);
    if (!output.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qCritical().noquote() << SAGE_FONT_PROBE_ERROR_OPEN_OUTPUT.arg(outputPath);
        return EXIT_FAILURE;
    }

    const QByteArray json = QJsonDocument(report).toJson(QJsonDocument::Indented);
    if (output.write(json) != json.size()) {
        qCritical().noquote() << SAGE_FONT_PROBE_ERROR_WRITE_OUTPUT.arg(outputPath);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

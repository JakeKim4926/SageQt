#include "ui/panels/SageWorkflowInputPanel.h"

#include "SageTestWorkflowHandler.h"
#include "ui/style/SageFontRegistry.h"
#include "ui/style/SageStyle.h"
#include "ui/widgets/SageLabel.h"
#include "ui/widgets/SageLineEdit.h"

#include <QApplication>
#include <QColor>
#include <QDir>
#include <QImage>
#include <QList>
#include <QObject>
#include <QPoint>
#include <QString>
#include <QTest>

class SageWorkflowInputPanelTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void handlerSetsInputLabel();
    void pathsShowNativeSeparatorsAndReturnQtPaths();
    void pathEditsAreReadOnlyAndIgnoreDrops();
    void drawsCardHeaderAndPathField();

private:
    static QList<SageLineEdit*> edits(SageWorkflowInputPanel& panel);
};

void SageWorkflowInputPanelTest::initTestCase()
{
    QString error;
    QVERIFY2(SageFontRegistry::registerApplicationFonts(error), qPrintable(error));
    QApplication::setStyle(new SageStyle);
    QApplication::setPalette(QApplication::style()->standardPalette());
}

QList<SageLineEdit*> SageWorkflowInputPanelTest::edits(SageWorkflowInputPanel& panel)
{
    return panel.findChildren<SageLineEdit*>();
}

void SageWorkflowInputPanelTest::handlerSetsInputLabel()
{
    SageWorkflowInputPanel panel;
    const SageTestWorkflowHandler handler(SageWorkflowType::Sample, QStringLiteral("테스트"), QStringLiteral("샘플"),
                                          false);

    panel.applyHandler(handler);

    bool hasInputLabel = false;
    const QList<SageLabel*> labels = panel.findChildren<SageLabel*>();
    for (const SageLabel* label : labels) {
        hasInputLabel = hasInputLabel || label->text() == handler.inputSectionLabel();
    }
    QVERIFY(hasInputLabel);
}

void SageWorkflowInputPanelTest::pathsShowNativeSeparatorsAndReturnQtPaths()
{
    SageWorkflowInputPanel panel;
    const QString inputPath = QDir::temp().filePath(QStringLiteral("input.xlsx"));

    panel.setInputPath(inputPath);
    panel.setOutputFolder(QDir::tempPath());

    QCOMPARE(edits(panel).at(0)->text(), QDir::toNativeSeparators(inputPath));
    QCOMPARE(panel.inputPath(), inputPath);
    QCOMPARE(panel.outputFolder(), QDir::tempPath());
}

void SageWorkflowInputPanelTest::pathEditsAreReadOnlyAndIgnoreDrops()
{
    SageWorkflowInputPanel panel;

    const QList<SageLineEdit*> pathEdits = edits(panel);
    for (const SageLineEdit* edit : pathEdits) {
        QVERIFY(edit->isReadOnly());
        QVERIFY(!edit->acceptDrops());
    }
}

void SageWorkflowInputPanelTest::drawsCardHeaderAndPathField()
{
    SageWorkflowInputPanel panel;
    panel.resize(700, 300);
    panel.show();
    QVERIFY(QTest::qWaitForWindowExposed(&panel));
    const SageLineEdit* inputEdit = edits(panel).at(0);
    const QPoint editTopLeft = inputEdit->mapTo(&panel, QPoint());

    const QImage image = panel.grab().toImage();

    QCOMPARE(image.pixelColor(0, 5), QColor(220, 214, 205));
    QCOMPARE(image.pixelColor(300, 5), QColor(242, 238, 231));
    QCOMPARE(image.pixelColor(300, 38), QColor(220, 214, 205));
    QCOMPARE(image.pixelColor(300, 39), QColor(255, 255, 255));
    QCOMPARE(editTopLeft.y(), 39 + 16);
    QCOMPARE(image.pixelColor(editTopLeft + QPoint(inputEdit->width() / 2, inputEdit->height() / 2)),
             QColor(248, 246, 241));
    QCOMPARE(image.pixelColor(edits(panel).at(1)->mapTo(&panel, QPoint())), QColor(220, 214, 205));
}

QTEST_MAIN(SageWorkflowInputPanelTest)

#include "SageWorkflowInputPanelTest.moc"

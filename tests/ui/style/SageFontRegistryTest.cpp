#include "ui/style/SageFontRegistry.h"

#include "ui/style/SageFontCatalog.h"

#include <QFont>
#include <QFontDatabase>
#include <QFontInfo>
#include <QObject>
#include <QString>
#include <QTest>

class SageFontRegistryTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void registersBundledFamilies();
    void pixelSizeConvertsTenthsOfPoint();
    void bodyResolvesToPretendardRegular();
    void bodyStrongResolvesToPretendardSemiBold();
    void titleResolvesToPretendardSemiBold();
    void sectionResolvesToPretendardSemiBold();
    void listResolvesToPretendardRegular();
    void captionResolvesToPretendardRegular();
    void summaryResolvesToPretendardSemiBold();
    void logoResolvesToGmarketSansBold();

private:
    static void compareRole(SageFontRole role, const QString& family, int weight, int pixelSize);
};

void SageFontRegistryTest::initTestCase()
{
    QString error;
    QVERIFY2(SageFontRegistry::registerApplicationFonts(error), qPrintable(error));
}

void SageFontRegistryTest::registersBundledFamilies()
{
    const QStringList families = QFontDatabase::families();

    QVERIFY(families.contains(QStringLiteral("Pretendard")));
    QVERIFY(families.contains(QStringLiteral("Gmarket Sans TTF")));
}

void SageFontRegistryTest::pixelSizeConvertsTenthsOfPoint()
{
    QCOMPARE(SageFontCatalog::pixelSize(105), 14);
    QCOMPARE(SageFontCatalog::pixelSize(143), 19);
    QCOMPARE(SageFontCatalog::pixelSize(113), 15);
    QCOMPARE(SageFontCatalog::pixelSize(98), 13);
    QCOMPARE(SageFontCatalog::pixelSize(90), 12);
    QCOMPARE(SageFontCatalog::pixelSize(128), 17);
}

void SageFontRegistryTest::compareRole(SageFontRole role, const QString& family, int weight, int pixelSize)
{
    const QFont font = SageFontCatalog::font(role);
    const QFontInfo info(font);

    QCOMPARE(font.pixelSize(), pixelSize);
    QCOMPARE(info.family(), family);
    QCOMPARE(info.weight(), weight);
}

void SageFontRegistryTest::bodyResolvesToPretendardRegular()
{
    compareRole(SageFontRole::Body, QStringLiteral("Pretendard"), 400, 14);
}

void SageFontRegistryTest::bodyStrongResolvesToPretendardSemiBold()
{
    compareRole(SageFontRole::BodyStrong, QStringLiteral("Pretendard"), 600, 14);
}

void SageFontRegistryTest::titleResolvesToPretendardSemiBold()
{
    compareRole(SageFontRole::Title, QStringLiteral("Pretendard"), 600, 19);
}

void SageFontRegistryTest::sectionResolvesToPretendardSemiBold()
{
    compareRole(SageFontRole::Section, QStringLiteral("Pretendard"), 600, 15);
}

void SageFontRegistryTest::listResolvesToPretendardRegular()
{
    compareRole(SageFontRole::List, QStringLiteral("Pretendard"), 400, 13);
}

void SageFontRegistryTest::captionResolvesToPretendardRegular()
{
    compareRole(SageFontRole::Caption, QStringLiteral("Pretendard"), 400, 12);
}

void SageFontRegistryTest::summaryResolvesToPretendardSemiBold()
{
    compareRole(SageFontRole::Summary, QStringLiteral("Pretendard"), 600, 17);
}

void SageFontRegistryTest::logoResolvesToGmarketSansBold()
{
    compareRole(SageFontRole::Logo, QStringLiteral("Gmarket Sans TTF"), 700, 19);
}

QTEST_MAIN(SageFontRegistryTest)

#include "SageFontRegistryTest.moc"

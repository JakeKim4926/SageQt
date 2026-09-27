#include "core/workflow/SageWorkflowRegistry.h"

#include "SageDefine.h"
#include "core/workflow/ISageWorkflowHandler.h"

#include <QObject>
#include <QTest>

class SageWorkflowRegistryTest : public QObject
{
    Q_OBJECT

private slots:
    void findHandlerReturnsSampleHandler();
    void findHandlerReturnsNullForUnregisteredType();
};

void SageWorkflowRegistryTest::findHandlerReturnsSampleHandler()
{
    const SageWorkflowRegistry registry;

    const ISageWorkflowHandler* handler = registry.findHandler(SageWorkflowType::Sample);

    QVERIFY(handler != nullptr);
    QCOMPARE(handler->workflowType(), SageWorkflowType::Sample);
}

void SageWorkflowRegistryTest::findHandlerReturnsNullForUnregisteredType()
{
    const SageWorkflowRegistry registry;

    QVERIFY(registry.findHandler(static_cast<SageWorkflowType>(0)) == nullptr);
}

QTEST_GUILESS_MAIN(SageWorkflowRegistryTest)

#include "SageWorkflowRegistryTest.moc"

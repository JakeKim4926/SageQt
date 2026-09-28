#include "core/workflow/SageWorkflowRegistry.h"

#include "SageDefine.h"
#include "core/workflow/ISageWorkflowHandler.h"
#include "core/workflow/handlers/SageSampleWorkflowHandler.h"

#include <QList>

#include <QObject>
#include <QTest>

#include <memory>

class SageWorkflowRegistryTest : public QObject
{
    Q_OBJECT

private slots:
    void findHandlerReturnsSampleHandler();
    void findHandlerReturnsNullForUnregisteredType();
    void handlersListsRegisteredHandlersInOrder();
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

void SageWorkflowRegistryTest::handlersListsRegisteredHandlersInOrder()
{
    SageWorkflowRegistry registry;
    registry.registerHandler(std::make_unique<SageSampleWorkflowHandler>());

    const QList<const ISageWorkflowHandler*> handlers = registry.handlers();

    QCOMPARE(handlers.size(), 2);
    QCOMPARE(handlers.at(0), registry.findHandler(SageWorkflowType::Sample));
    QVERIFY(handlers.at(1) != handlers.at(0));
}

QTEST_GUILESS_MAIN(SageWorkflowRegistryTest)

#include "SageWorkflowRegistryTest.moc"

#include "ui/panels/SageSidebarPanel.h"

#include "SageDefine.h"
#include "SageTestWorkflowHandler.h"
#include "core/auth/SageAuthSession.h"
#include "core/auth/SageUserDto.h"
#include "core/workflow/SageWorkflowRegistry.h"
#include "ui/style/SageFontRegistry.h"
#include "ui/style/SageStyle.h"
#include "ui/widgets/SageLabel.h"

#include <QAbstractItemModel>
#include <QApplication>
#include <QColor>
#include <QDialog>
#include <QImage>
#include <QModelIndex>
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QSignalSpy>
#include <QString>
#include <QTest>
#include <QTimer>
#include <QTreeView>

#include <memory>

class SageSidebarPanelTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void treeGroupsRegisteredHandlersByCategory();
    void startsWithFirstWorkflowSelected();
    void selectingOtherWorkflowEmitsOnce();
    void loginRequiredWorkflowWarnsAndRestores();
    void changePasswordWhileLoggedOutWarnsAndRestores();
    void changePasswordWhileLoggedInRequestsAndRestores();
    void clickingGroupKeepsWorkflowSelected();
    void drawsSidebarSurfaceDividerAndSelection();

private:
    QTreeView* tree(SageSidebarPanel& panel) const;
    QModelIndex itemIndex(SageSidebarPanel& panel, int groupRow, int row) const;
    void closeNextModal();

private:
    std::unique_ptr<SageWorkflowRegistry> m_registry;
    SageAuthSession m_authSession;
    QString m_modalTitle;
    int m_modalCount = 0;
};

static const SageWorkflowType SAGE_TEST_WORKFLOW = static_cast<SageWorkflowType>(2);
static const SageWorkflowType SAGE_LOGIN_WORKFLOW = static_cast<SageWorkflowType>(3);

void SageSidebarPanelTest::initTestCase()
{
    QString error;
    QVERIFY2(SageFontRegistry::registerApplicationFonts(error), qPrintable(error));
    QApplication::setStyle(new SageStyle);
}

void SageSidebarPanelTest::init()
{
    m_registry = std::make_unique<SageWorkflowRegistry>();
    m_registry->registerHandler(std::make_unique<SageTestWorkflowHandler>(
        SAGE_TEST_WORKFLOW, QStringLiteral("테스트 업무"), QStringLiteral("샘플"), false));
    m_registry->registerHandler(std::make_unique<SageTestWorkflowHandler>(
        SAGE_LOGIN_WORKFLOW, QStringLiteral("로그인 업무"), QStringLiteral("관리"), true));
    m_authSession.logout();
    m_modalTitle.clear();
    m_modalCount = 0;
}

QTreeView* SageSidebarPanelTest::tree(SageSidebarPanel& panel) const
{
    return panel.findChild<QTreeView*>();
}

QModelIndex SageSidebarPanelTest::itemIndex(SageSidebarPanel& panel, int groupRow, int row) const
{
    const QAbstractItemModel* model = tree(panel)->model();
    return model->index(row, 0, model->index(groupRow, 0));
}

void SageSidebarPanelTest::closeNextModal()
{
    QTimer::singleShot(0, this, [this]() {
        QDialog* dialog = qobject_cast<QDialog*>(QApplication::activeModalWidget());
        if (dialog != nullptr) {
            m_modalTitle = dialog->windowTitle();
            ++m_modalCount;
            dialog->reject();
        }
    });
}

void SageSidebarPanelTest::treeGroupsRegisteredHandlersByCategory()
{
    SageSidebarPanel panel(*m_registry, m_authSession);
    const QAbstractItemModel* model = tree(panel)->model();

    QCOMPARE(model->rowCount(), 3);
    QCOMPARE(model->index(0, 0).data().toString(), QStringLiteral("샘플"));
    QCOMPARE(itemIndex(panel, 0, 0).data().toString(), QStringLiteral("샘플 업무"));
    QCOMPARE(itemIndex(panel, 0, 1).data().toString(), QStringLiteral("테스트 업무"));
    QCOMPARE(model->index(1, 0).data().toString(), QStringLiteral("관리"));
    QCOMPARE(itemIndex(panel, 1, 0).data().toString(), QStringLiteral("로그인 업무"));
    QCOMPARE(model->index(2, 0).data().toString(), QStringLiteral("기타"));
    QCOMPARE(itemIndex(panel, 2, 0).data().toString(), QStringLiteral("비밀번호 변경"));
}

void SageSidebarPanelTest::startsWithFirstWorkflowSelected()
{
    SageSidebarPanel panel(*m_registry, m_authSession);

    QCOMPARE(panel.selectedWorkflow(), std::optional<SageWorkflowType>(SageWorkflowType::Sample));
    QCOMPARE(tree(panel)->currentIndex(), itemIndex(panel, 0, 0));
}

void SageSidebarPanelTest::selectingOtherWorkflowEmitsOnce()
{
    SageSidebarPanel panel(*m_registry, m_authSession);
    QSignalSpy workflowSpy(&panel, &SageSidebarPanel::workflowSelected);

    tree(panel)->setCurrentIndex(itemIndex(panel, 0, 1));
    tree(panel)->setCurrentIndex(itemIndex(panel, 0, 1));

    QCOMPARE(workflowSpy.count(), 1);
    QCOMPARE(workflowSpy.at(0).at(0).value<SageWorkflowType>(), SAGE_TEST_WORKFLOW);
    QCOMPARE(panel.selectedWorkflow(), std::optional<SageWorkflowType>(SAGE_TEST_WORKFLOW));
}

void SageSidebarPanelTest::loginRequiredWorkflowWarnsAndRestores()
{
    SageSidebarPanel panel(*m_registry, m_authSession);
    QSignalSpy workflowSpy(&panel, &SageSidebarPanel::workflowSelected);
    closeNextModal();

    tree(panel)->setCurrentIndex(itemIndex(panel, 1, 0));

    QCOMPARE(m_modalCount, 1);
    QCOMPARE(m_modalTitle, QStringLiteral("경고"));
    QCOMPARE(workflowSpy.count(), 0);
    QCOMPARE(tree(panel)->currentIndex(), itemIndex(panel, 0, 0));
    QCOMPARE(panel.selectedWorkflow(), std::optional<SageWorkflowType>(SageWorkflowType::Sample));
}

void SageSidebarPanelTest::changePasswordWhileLoggedOutWarnsAndRestores()
{
    SageSidebarPanel panel(*m_registry, m_authSession);
    QSignalSpy passwordSpy(&panel, &SageSidebarPanel::passwordChangeRequested);
    closeNextModal();

    tree(panel)->setCurrentIndex(itemIndex(panel, 2, 0));

    QCOMPARE(m_modalCount, 1);
    QCOMPARE(passwordSpy.count(), 0);
    QCOMPARE(tree(panel)->currentIndex(), itemIndex(panel, 0, 0));
}

void SageSidebarPanelTest::changePasswordWhileLoggedInRequestsAndRestores()
{
    m_authSession.setLogin(SageUserDto());
    SageSidebarPanel panel(*m_registry, m_authSession);
    QSignalSpy passwordSpy(&panel, &SageSidebarPanel::passwordChangeRequested);
    QSignalSpy workflowSpy(&panel, &SageSidebarPanel::workflowSelected);
    closeNextModal();

    tree(panel)->setCurrentIndex(itemIndex(panel, 2, 0));
    QCoreApplication::processEvents();

    QCOMPARE(m_modalCount, 0);
    QCOMPARE(passwordSpy.count(), 1);
    QCOMPARE(workflowSpy.count(), 0);
    QCOMPARE(tree(panel)->currentIndex(), itemIndex(panel, 0, 0));
}

void SageSidebarPanelTest::clickingGroupKeepsWorkflowSelected()
{
    SageSidebarPanel panel(*m_registry, m_authSession);
    panel.show();
    QVERIFY(QTest::qWaitForWindowExposed(&panel));
    QTreeView* view = tree(panel);

    QTest::mouseClick(view->viewport(), Qt::LeftButton, Qt::NoModifier,
                      view->visualRect(view->model()->index(0, 0)).center());

    QVERIFY(view->selectionModel()->isSelected(itemIndex(panel, 0, 0)));
    QCOMPARE(panel.selectedWorkflow(), std::optional<SageWorkflowType>(SageWorkflowType::Sample));
}

void SageSidebarPanelTest::drawsSidebarSurfaceDividerAndSelection()
{
    SageSidebarPanel panel(*m_registry, m_authSession);
    panel.resize(panel.width(), 600);
    panel.show();
    QVERIFY(QTest::qWaitForWindowExposed(&panel));
    const QTreeView* view = tree(panel);
    const QRect selectedRow = view->visualRect(itemIndex(panel, 0, 0)).translated(view->mapTo(&panel, QPoint()));

    const QImage image = panel.grab().toImage();

    QCOMPARE(panel.width(), 220);
    QCOMPARE(image.pixelColor(200, 10), QColor(36, 31, 26));
    QCOMPARE(image.pixelColor(200, 56), QColor(51, 44, 37));
    QCOMPARE(view->mapTo(&panel, QPoint()).y(), 72);
    QCOMPARE(selectedRow.height(), 34);
    QCOMPARE(image.pixelColor(1, selectedRow.center().y()), QColor(154, 107, 63));
    QCOMPARE(image.pixelColor(200, selectedRow.center().y()), QColor(58, 49, 41));
    QCOMPARE(panel.findChild<SageLabel*>()->font().family(), QStringLiteral("Gmarket Sans TTF"));
}

QTEST_MAIN(SageSidebarPanelTest)

#include "SageSidebarPanelTest.moc"

#include "ui/panels/SageHeaderPanel.h"

#include "SageDefine.h"
#include "core/auth/SageAuthSession.h"
#include "core/auth/SageUserDto.h"
#include "core/workflow/SageWorkflowRegistry.h"
#include "ui/style/SageFontRegistry.h"
#include "ui/style/SageStyle.h"
#include "ui/widgets/SageBadge.h"
#include "ui/widgets/SageButton.h"
#include "ui/widgets/SageLabel.h"

#include <QApplication>
#include <QColor>
#include <QImage>
#include <QList>
#include <QObject>
#include <QPoint>
#include <QSignalSpy>
#include <QString>
#include <QTest>

class SageHeaderPanelTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void loggedOutShowsOnlyLoginButton();
    void loginShowsUserIdAndRole();
    void logoutButtonLogsOut();
    void loginButtonRequestsLogin();
    void showWorkflowSetsTitleAndCategory();
    void drawsPanelBottomLineAndBadge();

private:
    static SageButton* button(SageHeaderPanel& panel, const QString& text);
    static SageLabel* label(SageHeaderPanel& panel, SageLabel::SageLabelVariant variant);
    static SageUserDto user(const QString& loginId, SageUserRole role);

private:
    SageWorkflowRegistry m_registry;
};

void SageHeaderPanelTest::initTestCase()
{
    QString error;
    QVERIFY2(SageFontRegistry::registerApplicationFonts(error), qPrintable(error));
    QApplication::setStyle(new SageStyle);
    QApplication::setPalette(QApplication::style()->standardPalette());
}

SageButton* SageHeaderPanelTest::button(SageHeaderPanel& panel, const QString& text)
{
    const QList<SageButton*> buttons = panel.findChildren<SageButton*>();
    for (SageButton* candidate : buttons) {
        if (candidate->text() == text) {
            return candidate;
        }
    }
    return nullptr;
}

SageLabel* SageHeaderPanelTest::label(SageHeaderPanel& panel, SageLabel::SageLabelVariant variant)
{
    const QList<SageLabel*> labels = panel.findChildren<SageLabel*>();
    for (SageLabel* candidate : labels) {
        if (candidate->variant() == variant) {
            return candidate;
        }
    }
    return nullptr;
}

SageUserDto SageHeaderPanelTest::user(const QString& loginId, SageUserRole role)
{
    SageUserDto dto;
    dto.m_loginId = loginId;
    dto.m_role = role;
    return dto;
}

void SageHeaderPanelTest::loggedOutShowsOnlyLoginButton()
{
    SageAuthSession session;
    SageHeaderPanel panel(m_registry, session);

    QVERIFY(!button(panel, QStringLiteral("로그인"))->isHidden());
    QVERIFY(button(panel, QStringLiteral("로그아웃"))->isHidden());
    QVERIFY(label(panel, SageLabel::SageLabelVariant::MutedCaption)->isHidden());
    QVERIFY(panel.findChild<SageBadge*>()->isHidden());
}

void SageHeaderPanelTest::loginShowsUserIdAndRole()
{
    SageAuthSession session;
    SageHeaderPanel panel(m_registry, session);

    session.setLogin(user(QStringLiteral("admin"), SageUserRole::Admin));

    QVERIFY(button(panel, QStringLiteral("로그인"))->isHidden());
    QVERIFY(!button(panel, QStringLiteral("로그아웃"))->isHidden());
    QCOMPARE(label(panel, SageLabel::SageLabelVariant::MutedCaption)->text(), QStringLiteral("admin"));
    QCOMPARE(panel.findChild<SageBadge*>()->text(), QStringLiteral("관리자"));

    session.setLogin(user(QStringLiteral("kim"), SageUserRole::User));

    QCOMPARE(panel.findChild<SageBadge*>()->text(), QStringLiteral("사용자"));
}

void SageHeaderPanelTest::logoutButtonLogsOut()
{
    SageAuthSession session;
    session.setLogin(user(QStringLiteral("admin"), SageUserRole::Admin));
    SageHeaderPanel panel(m_registry, session);

    button(panel, QStringLiteral("로그아웃"))->click();

    QVERIFY(!session.isLoggedIn());
    QVERIFY(!button(panel, QStringLiteral("로그인"))->isHidden());
    QVERIFY(panel.findChild<SageBadge*>()->isHidden());
}

void SageHeaderPanelTest::loginButtonRequestsLogin()
{
    SageAuthSession session;
    SageHeaderPanel panel(m_registry, session);
    QSignalSpy loginSpy(&panel, &SageHeaderPanel::loginRequested);

    button(panel, QStringLiteral("로그인"))->click();

    QCOMPARE(loginSpy.count(), 1);
    QVERIFY(!session.isLoggedIn());
}

void SageHeaderPanelTest::showWorkflowSetsTitleAndCategory()
{
    SageAuthSession session;
    SageHeaderPanel panel(m_registry, session);

    panel.showWorkflow(SageWorkflowType::Sample);

    QCOMPARE(label(panel, SageLabel::SageLabelVariant::Title)->text(), QStringLiteral("샘플 업무"));
    QCOMPARE(label(panel, SageLabel::SageLabelVariant::SecondaryCaption)->text(), QStringLiteral("샘플"));
}

void SageHeaderPanelTest::drawsPanelBottomLineAndBadge()
{
    SageAuthSession session;
    session.setLogin(user(QStringLiteral("admin"), SageUserRole::Admin));
    SageHeaderPanel panel(m_registry, session);
    panel.resize(1000, panel.sizeHint().height());
    panel.show();
    QVERIFY(QTest::qWaitForWindowExposed(&panel));
    const SageBadge* badge = panel.findChild<SageBadge*>();
    const QPoint badgeTopLeft = badge->mapTo(&panel, QPoint());

    const QImage image = panel.grab().toImage();

    QCOMPARE(panel.height(), 57);
    QCOMPARE(image.pixelColor(5, 5), QColor(255, 255, 255));
    QCOMPARE(image.pixelColor(5, 56), QColor(220, 214, 205));
    QCOMPARE(badge->height(), 20);
    QCOMPARE(badgeTopLeft.y(), 18);
    QCOMPARE(image.pixelColor(badgeTopLeft + QPoint(0, 0)), QColor(255, 255, 255));
    QCOMPARE(image.pixelColor(badgeTopLeft + QPoint(badge->width() / 2, 2)), QColor(242, 238, 231));
    QCOMPARE(button(panel, QStringLiteral("로그아웃"))->height(), 32);
    QVERIFY(button(panel, QStringLiteral("로그아웃"))->width() >= 68);
}

QTEST_MAIN(SageHeaderPanelTest)

#include "SageHeaderPanelTest.moc"

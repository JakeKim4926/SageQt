#include "core/auth/SageAuthSession.h"

#include "SageDefine.h"

#include <QObject>
#include <QTest>

class SageAuthSessionTest : public QObject
{
    Q_OBJECT

private slots:
    void startsLoggedOut();
    void setLoginStoresUserAndRole();
    void logoutClearsUser();
};

void SageAuthSessionTest::startsLoggedOut()
{
    const SageAuthSession session;

    QCOMPARE(session.isLoggedIn(), false);
    QCOMPARE(session.isAdmin(), false);
}

void SageAuthSessionTest::setLoginStoresUserAndRole()
{
    SageAuthSession session;
    SageUserDto user;
    user.m_userId = 7;
    user.m_loginId = QStringLiteral("admin");
    user.m_role = SageUserRole::Admin;

    session.setLogin(user);

    QCOMPARE(session.isLoggedIn(), true);
    QCOMPARE(session.isAdmin(), true);
    QCOMPARE(session.currentUser().m_userId, 7);
    QCOMPARE(session.currentUser().m_loginId, QStringLiteral("admin"));
}

void SageAuthSessionTest::logoutClearsUser()
{
    SageAuthSession session;
    SageUserDto user;
    user.m_userId = 7;
    user.m_role = SageUserRole::Admin;
    session.setLogin(user);

    session.logout();

    QCOMPARE(session.isLoggedIn(), false);
    QCOMPARE(session.isAdmin(), false);
    QCOMPARE(session.currentUser().m_userId, 0);
}

QTEST_GUILESS_MAIN(SageAuthSessionTest)

#include "SageAuthSessionTest.moc"

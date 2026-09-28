#include "SageDefine.h"
#include "core/auth/SageAuthSession.h"
#include "core/auth/SageUserService.h"
#include "infra/auth/SagePbkdf2PasswordHasher.h"
#include "infra/db/SageDbConfig.h"
#include "infra/db/SageSchemaInitializer.h"
#include "infra/db/SageUserRepository.h"
#include "ui/dialogs/SageMessageBoxDlg.h"
#include "ui/style/SageFontCatalog.h"
#include "ui/style/SageFontRegistry.h"
#include "ui/style/SageStyle.h"
#include "ui/window/SageMainWindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QLoggingCategory>
#include <QString>
#include <QStyle>
#include <QThreadPool>

#include <cstdlib>
#include <optional>

Q_STATIC_LOGGING_CATEGORY(sageAppLog, SAGE_LOG_CATEGORY_APP)

static int failStartup(const QString& error)
{
    qCCritical(sageAppLog).noquote() << error;
    SageMessageBoxDlg errorDialog(SageMessageIcon::Error, error);
    errorDialog.exec();
    return EXIT_FAILURE;
}

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    QCoreApplication::setOrganizationName(SAGE_ORGANIZATION_NAME);
    QCoreApplication::setApplicationName(SAGE_APPLICATION_NAME);

    QString error;
    if (!SageFontRegistry::registerApplicationFonts(error)) {
        qCWarning(sageAppLog).noquote() << error;
    }
    QApplication::setStyle(new SageStyle);
    QApplication::setPalette(QApplication::style()->standardPalette());
    QApplication::setFont(SageFontCatalog::font(SageFontRole::Body));

    SageDbConfig dbConfig;
    if (!SageDbConfig::buildDefaultConfig(dbConfig, error) || !SageSchemaInitializer(dbConfig).prepare(error)) {
        return failStartup(error);
    }

    const SageUserRepository userRepository(dbConfig);
    const SagePbkdf2PasswordHasher passwordHasher;
    const SageUserService userService(userRepository, passwordHasher);
    std::optional<QString> initialAdminPassword;
    if (!userService.ensureDefaultAdmin(initialAdminPassword, error)) {
        return failStartup(error);
    }
    if (initialAdminPassword.has_value()) {
        SageMessageBoxDlg noticeDialog(
            SageMessageIcon::Info, SAGE_UI_INITIAL_ADMIN_PW_FORMAT.arg(SAGE_DEFAULT_ADMIN_ID, *initialAdminPassword));
        noticeDialog.exec();
    }
    SageAuthSession authSession;

    SageMainWindow mainWindow;
    mainWindow.show();
    const int exitCode = QApplication::exec();

    QThreadPool::globalInstance()->waitForDone();
    return exitCode;
}

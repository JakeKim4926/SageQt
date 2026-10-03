#include "ui/dialogs/SagePasswordChangeDlg.h"

#include "SageDefine.h"
#include "core/auth/SageAuthSession.h"
#include "core/auth/SageUserDto.h"
#include "core/auth/SageUserService.h"
#include "ui/dialogs/SageMessageBoxDlg.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/widgets/SageButton.h"
#include "ui/widgets/SageInlineMessage.h"
#include "ui/widgets/SageLabel.h"
#include "ui/widgets/SageLineEdit.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QWidget>
#include <QtConcurrentRun>

#include <optional>

SagePasswordChangeDlg::SagePasswordChangeDlg(const SageUserService& userService, SageAuthSession& authSession,
                                             QWidget* parent)
    : SageFramelessDlg(SAGE_UI_CHANGE_PW_TITLE, parent)
    , m_userService(userService)
    , m_authSession(authSession)
{
    setMinimumWidth(SAGE_PASSWORD_DLG_WIDTH);
    createWidgets();
    createLayout();
    connectSignals();
}

void SagePasswordChangeDlg::onChangeButtonClicked()
{
    m_errorMessage->clearMessage();
    m_currentEdit->setVariant(SageLineEdit::SageLineEditVariant::Normal);
    m_newEdit->setVariant(SageLineEdit::SageLineEditVariant::Normal);
    m_confirmEdit->setVariant(SageLineEdit::SageLineEditVariant::Normal);

    const QString currentPassword = m_currentEdit->text();
    const QString newPassword = m_newEdit->text();
    const QString confirmPassword = m_confirmEdit->text();
    if (currentPassword.isEmpty()) {
        showInputError(m_currentEdit, SAGE_UI_CHANGE_PW_EMPTY_CURRENT);
        return;
    }
    if (newPassword.isEmpty()) {
        showInputError(m_newEdit, SAGE_UI_CHANGE_PW_EMPTY_NEW);
        return;
    }
    if (confirmPassword.isEmpty()) {
        showInputError(m_confirmEdit, SAGE_UI_CHANGE_PW_EMPTY_CONFIRM);
        return;
    }
    if (newPassword != confirmPassword) {
        showInputError(m_confirmEdit, SAGE_UI_CHANGE_PW_MISMATCH);
        m_confirmEdit->selectAll();
        return;
    }

    m_changeButton->setEnabled(false);
    const SageUserService& userService = m_userService;
    const QString loginId = m_authSession.currentUser().m_loginId;
    const int userId = m_authSession.currentUser().m_userId;
    m_changeWatcher->setFuture(QtConcurrent::run([&userService, loginId, userId, currentPassword, newPassword]() {
        SagePasswordChangeAttempt attempt;
        std::optional<SageUserDto> verifiedUser;
        if (!userService.login(loginId, currentPassword, verifiedUser, attempt.m_error)) {
            attempt.m_stage = SagePasswordChangeStage::QueryFailed;
            return attempt;
        }
        if (!verifiedUser.has_value()) {
            attempt.m_stage = SagePasswordChangeStage::CurrentInvalid;
            return attempt;
        }
        attempt.m_stage = userService.changePassword(userId, newPassword, attempt.m_error)
                              ? SagePasswordChangeStage::Changed
                              : SagePasswordChangeStage::ChangeFailed;
        return attempt;
    }));
}

void SagePasswordChangeDlg::onChangeWatcherFinished()
{
    m_changeButton->setEnabled(true);
    const SagePasswordChangeAttempt attempt = m_changeWatcher->result();
    switch (attempt.m_stage) {
    case SagePasswordChangeStage::QueryFailed: {
        SageMessageBoxDlg errorDialog(SageMessageIcon::Error, attempt.m_error, this);
        errorDialog.exec();
        return;
    }
    case SagePasswordChangeStage::CurrentInvalid:
        showInputError(m_currentEdit, SAGE_UI_CHANGE_PW_CURRENT_INVALID);
        m_currentEdit->selectAll();
        return;
    case SagePasswordChangeStage::ChangeFailed:
        showInputError(m_newEdit, attempt.m_error);
        m_newEdit->selectAll();
        return;
    case SagePasswordChangeStage::Changed:
        break;
    }

    SageUserDto user = m_authSession.currentUser();
    user.m_isPasswordChangeRequired = false;
    m_authSession.setLogin(user);
    SageMessageBoxDlg completedDialog(SageMessageIcon::Info, SAGE_UI_CHANGE_PW_COMPLETED, this);
    completedDialog.exec();
    accept();
}

void SagePasswordChangeDlg::createWidgets()
{
    m_currentLabel = addFormLabel(SAGE_UI_CHANGE_PW_CURRENT);
    m_newLabel = addFormLabel(SAGE_UI_CHANGE_PW_NEW);
    m_confirmLabel = addFormLabel(SAGE_UI_CHANGE_PW_CONFIRM);

    m_currentEdit = addPasswordEdit();
    m_newEdit = addPasswordEdit();
    m_newEdit->setMaxLength(SAGE_USER_PW_MAX_LEN);
    m_confirmEdit = addPasswordEdit();
    m_confirmEdit->setMaxLength(SAGE_USER_PW_MAX_LEN);

    m_errorMessage = new SageInlineMessage(SageInlineMessage::SageInlineMessageVariant::Error, contentWidget());
    m_hintLabel = new SageLabel(SageLabel::SageLabelVariant::SecondaryCaption, SAGE_UI_CHANGE_PW_HINT, contentWidget());
    m_hintLabel->setFixedHeight(SAGE_INLINE_MSG_HEIGHT);

    m_buttonRow = new QWidget(contentWidget());
    m_changeButton = new SageButton(SAGE_UI_CHANGE_PW_OK, m_buttonRow);
    m_changeButton->setVariant(SageButton::SageButtonVariant::Primary);
    m_changeButton->setMinimumWidth(SAGE_LOGIN_DLG_BTN_WIDTH);
    m_changeButton->setDefault(true);
    m_cancelButton = new SageButton(SAGE_UI_CHANGE_PW_CANCEL, m_buttonRow);
    m_cancelButton->setMinimumWidth(SAGE_LOGIN_DLG_BTN_WIDTH);

    m_changeWatcher = new QFutureWatcher<SagePasswordChangeAttempt>(this);
}

void SagePasswordChangeDlg::createLayout()
{
    QGridLayout* layout = new QGridLayout(contentWidget());
    layout->setContentsMargins(SAGE_MARGIN, SAGE_MARGIN, SAGE_MARGIN, SAGE_MARGIN);
    layout->setHorizontalSpacing(SAGE_ROW_GAP);
    layout->setVerticalSpacing(0);
    layout->addWidget(m_currentLabel, 0, 0);
    layout->addWidget(m_currentEdit, 0, 1);
    layout->setRowMinimumHeight(1, SAGE_ROW_GAP);
    layout->addWidget(m_newLabel, 2, 0);
    layout->addWidget(m_newEdit, 2, 1);
    layout->setRowMinimumHeight(3, SAGE_ROW_GAP);
    layout->addWidget(m_confirmLabel, 4, 0);
    layout->addWidget(m_confirmEdit, 4, 1);
    layout->addWidget(m_errorMessage, 5, 1);
    layout->addWidget(m_hintLabel, 6, 1);
    layout->setRowMinimumHeight(7, SAGE_ROW_GAP);
    layout->addWidget(m_buttonRow, 8, 0, 1, 2);
    layout->setColumnStretch(1, 1);

    QHBoxLayout* buttonLayout = new QHBoxLayout(m_buttonRow);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(SAGE_ROW_GAP);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_changeButton);
    buttonLayout->addWidget(m_cancelButton);
}

void SagePasswordChangeDlg::connectSignals()
{
    connect(m_changeButton, &SageButton::clicked, this, &SagePasswordChangeDlg::onChangeButtonClicked);
    connect(m_cancelButton, &SageButton::clicked, this, &SagePasswordChangeDlg::reject);
    connect(m_changeWatcher, &QFutureWatcher<SagePasswordChangeAttempt>::finished, this,
            &SagePasswordChangeDlg::onChangeWatcherFinished);
}

void SagePasswordChangeDlg::showInputError(SageLineEdit* edit, const QString& message)
{
    m_errorMessage->setMessage(message);
    edit->setVariant(SageLineEdit::SageLineEditVariant::Error);
    edit->setFocus();
}

SageLabel* SagePasswordChangeDlg::addFormLabel(const QString& text)
{
    SageLabel* label = new SageLabel(SageLabel::SageLabelVariant::FormLabel, text, contentWidget());
    label->setMinimumWidth(SAGE_PASSWORD_DLG_LABEL_WIDTH);
    return label;
}

SageLineEdit* SagePasswordChangeDlg::addPasswordEdit()
{
    SageLineEdit* edit = new SageLineEdit(contentWidget());
    edit->setEchoMode(QLineEdit::Password);
    return edit;
}

#pragma once

#include "ui/dialogs/SageFramelessDlg.h"

#include <QFutureWatcher>
#include <QString>

class QWidget;
class SageAuthSession;
class SageButton;
class SageInlineMessage;
class SageLabel;
class SageLineEdit;
class SageUserService;

enum class SagePasswordChangeStage
{
    QueryFailed,
    CurrentInvalid,
    ChangeFailed,
    Changed
};

struct SagePasswordChangeAttempt
{
    SagePasswordChangeStage m_stage = SagePasswordChangeStage::QueryFailed;
    QString m_error;
};

class SagePasswordChangeDlg : public SageFramelessDlg
{
    Q_OBJECT

public:
    SagePasswordChangeDlg(const SageUserService& userService, SageAuthSession& authSession, QWidget* parent = nullptr);

private slots:
    void onChangeButtonClicked();
    void onChangeFinished();

private:
    void createWidgets();
    void createLayout();
    void connectSignals();
    void showInputError(SageLineEdit* edit, const QString& message);
    SageLabel* createFormLabel(const QString& text);
    SageLineEdit* createPasswordEdit();

private:
    const SageUserService& m_userService;
    SageAuthSession& m_authSession;
    SageLabel* m_currentLabel = nullptr;
    SageLabel* m_newLabel = nullptr;
    SageLabel* m_confirmLabel = nullptr;
    SageLineEdit* m_currentEdit = nullptr;
    SageLineEdit* m_newEdit = nullptr;
    SageLineEdit* m_confirmEdit = nullptr;
    SageInlineMessage* m_errorMessage = nullptr;
    SageLabel* m_hintLabel = nullptr;
    QWidget* m_buttonRow = nullptr;
    SageButton* m_changeButton = nullptr;
    SageButton* m_cancelButton = nullptr;
    QFutureWatcher<SagePasswordChangeAttempt>* m_changeWatcher = nullptr;
};

#pragma once

#include "SageDefine.h"

#include <QString>
#include <QWidget>

class ISageWorkflowHandler;
class QFrame;
class SageButton;
class SageLabel;
class QTimer;
class SageLineEdit;
class SageLabel;
class SageResultTablePanel;
class SageStatusCard;

class SageWorkflowInputPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SageWorkflowInputPanel(QWidget* parent = nullptr);

    QString inputPath() const;
    void setInputPath(const QString& inputPath);
    QString outputFolder() const;
    void setOutputFolder(const QString& outputFolder);
    void applyHandler(const ISageWorkflowHandler& handler);
    void setRunningState(bool running);
    void setStatusResult(bool success, const QString& message, const QString& detail);
    void resetStatusCard();
    void setGenerateEnabled(bool enabled);
    void setInputResetVisible(bool visible);
    void setInputTableVisible(bool tableVisible, bool filterVisible);
    SageResultTablePanel& inputTablePanel();

signals:
    void runRequested(SageTaskType taskType);
    void openOutputFolderRequested();
    void inputResetRequested();

private slots:
    void onSelectInputButtonClicked();
    void onSelectOutputButtonClicked();
    void onRunButtonClicked();
    void onProgressTimerTimeout();

private:
    void createWidgets();
    void createLayout();
    void connectSignals();
    SageLineEdit* addPathEdit();

private:
    QFrame* m_inputCard = nullptr;
    SageLabel* m_cardTitleLabel = nullptr;
    QFrame* m_cardTitleLine = nullptr;
    QWidget* m_formArea = nullptr;
    SageLabel* m_inputLabel = nullptr;
    SageLabel* m_outputLabel = nullptr;
    SageLineEdit* m_inputPathEdit = nullptr;
    SageLineEdit* m_outputFolderEdit = nullptr;
    SageButton* m_selectInputButton = nullptr;
    SageButton* m_selectOutputButton = nullptr;
    SageButton* m_runButton = nullptr;
    SageButton* m_inputResetButton = nullptr;
    SageStatusCard* m_statusCard = nullptr;
    SageResultTablePanel* m_inputTablePanel = nullptr;
    QWidget* m_emptyHintArea = nullptr;
    SageLabel* m_emptyHintLabel = nullptr;
    bool m_isInputTableVisible = false;
    QTimer* m_progressTimer = nullptr;
    int m_progressPercent = 0;
    bool m_isRunning = false;
    QString m_inputDialogTitle;
    QString m_inputFileFilter;
    bool m_isAutoLoadOnInput = false;
};

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

signals:
    void runRequested(SageTaskType taskType);
    void openOutputFolderRequested();

private slots:
    void onSelectInputClicked();
    void onSelectOutputClicked();
    void onRunClicked();
    void onProgressTimer();

private:
    void createWidgets();
    void createLayout();
    void connectSignals();
    SageLineEdit* createPathEdit();

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
    SageStatusCard* m_statusCard = nullptr;
    QTimer* m_progressTimer = nullptr;
    int m_progressPercent = 0;
    bool m_running = false;
    QString m_inputDialogTitle;
    QString m_inputFileFilter;
    bool m_autoLoadOnInput = false;
};

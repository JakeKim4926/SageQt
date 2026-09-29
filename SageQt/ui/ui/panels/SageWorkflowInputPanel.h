#pragma once

#include <QString>
#include <QWidget>

class ISageWorkflowHandler;
class QFrame;
class SageButton;
class SageLabel;
class SageLineEdit;

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

private slots:
    void onSelectInputClicked();
    void onSelectOutputClicked();

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
    QString m_inputDialogTitle;
    QString m_inputFileFilter;
};

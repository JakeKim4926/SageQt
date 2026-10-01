#pragma once

#include <QWidget>

class SageResultTablePanel;

class SageWorkflowResultPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SageWorkflowResultPanel(QWidget* parent = nullptr);

    SageResultTablePanel& resultTable();

private:
    SageResultTablePanel* m_resultTable = nullptr;
};

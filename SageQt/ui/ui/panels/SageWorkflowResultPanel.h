#pragma once

#include <QWidget>

class SageResultTablePanel;

class SageWorkflowResultPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SageWorkflowResultPanel(QWidget* parent = nullptr);

    SageResultTablePanel& resultTable();
    void setFilterVisible(bool visible);

private:
    SageResultTablePanel* m_resultTable = nullptr;
};

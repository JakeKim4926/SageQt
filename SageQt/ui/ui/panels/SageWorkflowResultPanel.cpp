#include "ui/panels/SageWorkflowResultPanel.h"

#include "SageDefine.h"
#include "ui/panels/SageResultTablePanel.h"

#include <QVBoxLayout>

SageWorkflowResultPanel::SageWorkflowResultPanel(QWidget* parent)
    : QWidget(parent)
{
    m_resultTable = new SageResultTablePanel(SAGE_UI_SECTION_RESULT, this);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_resultTable);
}

SageResultTablePanel& SageWorkflowResultPanel::resultTable()
{
    return *m_resultTable;
}

void SageWorkflowResultPanel::setFilterVisible(bool visible)
{
    m_resultTable->showFilter(visible);
}

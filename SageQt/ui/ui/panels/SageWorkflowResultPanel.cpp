#include "ui/panels/SageWorkflowResultPanel.h"

#include "SageDefine.h"
#include "ui/panels/SageResultTablePanel.h"

#include <QVBoxLayout>

SageWorkflowResultPanel::SageWorkflowResultPanel(QWidget* parent)
    : QWidget(parent)
{
    m_resultTablePanel = new SageResultTablePanel(SAGE_UI_SECTION_RESULT, this);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_resultTablePanel);
}

SageResultTablePanel& SageWorkflowResultPanel::resultTablePanel()
{
    return *m_resultTablePanel;
}

void SageWorkflowResultPanel::setFilterVisible(bool visible)
{
    m_resultTablePanel->showFilter(visible);
}

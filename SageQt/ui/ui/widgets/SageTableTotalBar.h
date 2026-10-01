#pragma once

#include "core/workflow/SageWorkflowResultTable.h"

#include <QList>
#include <QSize>
#include <QString>
#include <QWidget>

class QPaintEvent;

struct SageTableTotalBarCell
{
    QString m_text;
    int m_left = 0;
    int m_width = 0;
    SageColumnAlign m_align = SageColumnAlign::Left;
    SageResultTotalRole m_role = SageResultTotalRole::Label;
};

class SageTableTotalBar : public QWidget
{
    Q_OBJECT

public:
    explicit SageTableTotalBar(QWidget* parent = nullptr);

    void setCells(const QList<SageTableTotalBarCell>& cells);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    static QColor cellColor(SageResultTotalRole role);

private:
    QList<SageTableTotalBarCell> m_cells;
};

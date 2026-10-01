#pragma once

#include <QList>
#include <QTableView>

class QAbstractItemModel;
class QEvent;
class SageResultTableDelegate;

struct SageTableColumnSpec
{
    int m_width = 0;
    bool m_isStretch = false;
};

class SageTableView : public QTableView
{
    Q_OBJECT

public:
    explicit SageTableView(QWidget* parent = nullptr);

    void setColumnSpecs(const QList<SageTableColumnSpec>& columnSpecs);
    void setRowSeparator(bool enabled);
    void setModel(QAbstractItemModel* model) override;

protected:
    bool viewportEvent(QEvent* event) override;

private:
    void applyColumnWidths();
    void setHoveredRow(int row);

private:
    SageResultTableDelegate* m_delegate = nullptr;
    QList<SageTableColumnSpec> m_columnSpecs;
};

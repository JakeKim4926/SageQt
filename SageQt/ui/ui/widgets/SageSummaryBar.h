#pragma once

#include "core/workflow/SageWorkflowResultTable.h"

#include <QList>
#include <QSize>
#include <QString>
#include <QWidget>

class QPainter;
class QPaintEvent;

class SageSummaryBar : public QWidget
{
    Q_OBJECT

public:
    explicit SageSummaryBar(QWidget* parent = nullptr);

    void setItems(const QList<SageResultSummaryItem>& items);
    bool hasItems() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int itemWidth(const SageResultSummaryItem& item) const;
    int drawSegment(QPainter& painter, int left, const QString& text, const QFont& font, const QColor& color) const;
    void drawBadge(QPainter& painter, int left, const SageResultSummaryItem& item) const;
    static QString badgeText(const SageResultSummaryItem& item);
    int firstBadgeIndex() const;

private:
    QList<SageResultSummaryItem> m_items;
};

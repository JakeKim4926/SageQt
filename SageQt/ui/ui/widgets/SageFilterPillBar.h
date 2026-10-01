#pragma once

#include <QList>
#include <QRect>
#include <QSize>
#include <QString>
#include <QStringList>
#include <QWidget>

class QMouseEvent;
class QPaintEvent;

class SageFilterPillBar : public QWidget
{
    Q_OBJECT

public:
    explicit SageFilterPillBar(QWidget* parent = nullptr);

    void setLabels(const QStringList& labels);
    int selectedIndex() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void selectedIndexChanged(int index);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    QList<QRect> pillRects() const;

private:
    QStringList m_labels;
    int m_selectedIndex = 0;
};

#pragma once

#include <QSize>
#include <QString>
#include <QWidget>

class QPaintEvent;

class SageSelectionCountLabel : public QWidget
{
    Q_OBJECT

public:
    explicit SageSelectionCountLabel(QWidget* parent = nullptr);

    void setCounts(int totalCount, int selectedCount);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString totalText() const;
    QString selectedText() const;

private:
    int m_totalCount = 0;
    int m_selectedCount = 0;
};

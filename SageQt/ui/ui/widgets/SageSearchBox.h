#pragma once

#include "core/workflow/SageWorkflowResultTable.h"

#include <QList>
#include <QRect>
#include <QSize>
#include <QString>
#include <QWidget>

#include <optional>

class QComboBox;
class QLineEdit;
class QMouseEvent;
class QPaintEvent;

class SageSearchBox : public QWidget
{
    Q_OBJECT

public:
    explicit SageSearchBox(QWidget* parent = nullptr);

    void setCriteria(const QList<SageWorkflowFilterCriteria>& criteria, int selectedCriteria);
    std::optional<int> selectedCriteria() const;
    QString keyword() const;
    void setKeyword(const QString& keyword);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void searchRequested();
    void criteriaChanged(int criteria);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void onCriteriaComboActivated(int index);

private:
    QRect criteriaCellRect() const;
    QRect iconCellRect() const;

private:
    QComboBox* m_criteriaCombo = nullptr;
    QLineEdit* m_keywordEdit = nullptr;
};

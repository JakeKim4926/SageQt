#pragma once

#include <QWidget>

class QCheckBox;
class SageButton;
class SageSelectionCountLabel;

class SageSelectionBar : public QWidget
{
    Q_OBJECT

public:
    explicit SageSelectionBar(QWidget* parent = nullptr);

    void setCounts(int totalCount, int selectedCount);
    void setAllChecked(bool checked);
    void setControlsEnabled(bool enabled);

signals:
    void selectAllClicked();
    void clearClicked();

private:
    QCheckBox* m_selectAllCheck = nullptr;
    SageSelectionCountLabel* m_countLabel = nullptr;
    SageButton* m_clearButton = nullptr;
};

#pragma once

#include <QSize>
#include <QStyledItemDelegate>

class QModelIndex;
class QPainter;
class QStyleOptionViewItem;

class SageSidebarDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit SageSidebarDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

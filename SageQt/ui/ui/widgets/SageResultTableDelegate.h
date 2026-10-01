#pragma once

#include "ui/models/SageTableRole.h"

#include <QRect>
#include <QString>
#include <QStyledItemDelegate>

class QAbstractItemModel;
class QEvent;
class QModelIndex;
class QPainter;
class QStyleOptionViewItem;

class SageResultTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit SageResultTableDelegate(QObject* parent = nullptr);

    void setRowSeparator(bool enabled);
    void setHoveredRow(int row);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

protected:
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option,
                     const QModelIndex& index) override;

private:
    static bool hasCheckBox(const QModelIndex& index);
    static QRect checkCellRect(const QRect& cellRect);
    static QRect textRect(const QRect& cellRect, const QModelIndex& index);
    static void drawCheckBox(QPainter* painter, const QRect& cellRect, bool checked);
    static QColor textColor(const QModelIndex& index);
    static void drawBadge(QPainter* painter, const QRect& contentRect, const QString& text, SageTableTone tone);

private:
    bool m_rowSeparator = false;
    int m_hoveredRow = -1;
};

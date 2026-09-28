#include "ui/widgets/SageSidebarDelegate.h"

#include "ui/models/SageSidebarModel.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageFontCatalog.h"

#include <QColor>
#include <QFont>
#include <QModelIndex>
#include <QPainter>
#include <QRect>
#include <QStyle>
#include <QStyleOptionViewItem>

SageSidebarDelegate::SageSidebarDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

void SageSidebarDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const bool isGroup = index.data(static_cast<int>(SageSidebarRole::ItemKind)).value<SageSidebarItemKind>() ==
                         SageSidebarItemKind::Group;
    const bool isSelected = !isGroup && option.state.testFlag(QStyle::State_Selected);

    painter->save();
    painter->fillRect(option.rect, isSelected ? SAGE_COLOR_SIDEBAR_SELECTED : SAGE_COLOR_SIDEBAR);
    if (isSelected) {
        QRect accent = option.rect;
        accent.setWidth(SAGE_SELECTION_ACCENT_WIDTH);
        painter->fillRect(accent, SAGE_COLOR_PRIMARY);
    }

    QFont font = SageFontCatalog::font(isGroup      ? SageFontRole::Caption
                                       : isSelected ? SageFontRole::BodyStrong
                                                    : SageFontRole::Body);
    QColor textColor = isSelected ? SAGE_COLOR_SIDEBAR_SELECTED_TEXT : SAGE_COLOR_SIDEBAR_TEXT;
    if (isGroup) {
        font.setLetterSpacing(QFont::AbsoluteSpacing, SAGE_SIDEBAR_CATEGORY_CHAR_EXTRA);
        textColor = SAGE_COLOR_SIDEBAR_CATEGORY;
    }
    painter->setFont(font);
    painter->setPen(textColor);
    painter->drawText(option.rect.adjusted(SAGE_SIDEBAR_PAD_X, 0, 0, 0), Qt::AlignLeft | Qt::AlignVCenter,
                      index.data(Qt::DisplayRole).toString());
    painter->restore();
}

QSize SageSidebarDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return {QStyledItemDelegate::sizeHint(option, index).width(), SAGE_SIDEBAR_ITEM_HEIGHT};
}

#include "ui/window/SageFileDropFilter.h"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QEvent>
#include <QList>
#include <QMimeData>
#include <QUrl>

SageFileDropFilter::SageFileDropFilter(QObject* parent)
    : QObject(parent)
{
}

bool SageFileDropFilter::eventFilter(QObject* watched, QEvent* event)
{
    switch (event->type()) {
    case QEvent::DragEnter:
    case QEvent::DragMove: {
        QDragMoveEvent* dragEvent = static_cast<QDragMoveEvent*>(event);
        if (!localFilePaths(dragEvent->mimeData()).isEmpty()) {
            dragEvent->acceptProposedAction();
            return true;
        }
        break;
    }
    case QEvent::Drop: {
        QDropEvent* dropEvent = static_cast<QDropEvent*>(event);
        const QStringList paths = localFilePaths(dropEvent->mimeData());
        if (!paths.isEmpty()) {
            dropEvent->acceptProposedAction();
            emit filesDropped(paths);
            return true;
        }
        break;
    }
    default:
        break;
    }
    return QObject::eventFilter(watched, event);
}

QStringList SageFileDropFilter::localFilePaths(const QMimeData* mimeData)
{
    QStringList paths;
    if (mimeData == nullptr || !mimeData->hasUrls()) {
        return paths;
    }
    const QList<QUrl> urls = mimeData->urls();
    for (const QUrl& url : urls) {
        if (url.isLocalFile()) {
            paths.append(url.toLocalFile());
        }
    }
    return paths;
}

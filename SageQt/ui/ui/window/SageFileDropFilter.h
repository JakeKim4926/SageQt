#pragma once

#include <QObject>
#include <QStringList>

class QEvent;
class QMimeData;

class SageFileDropFilter : public QObject
{
    Q_OBJECT

public:
    explicit SageFileDropFilter(QObject* parent = nullptr);

signals:
    void filesDropped(const QStringList& paths);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    static QStringList localFilePaths(const QMimeData* mimeData);
};

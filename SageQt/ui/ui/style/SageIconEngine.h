#pragma once

#include <QColor>
#include <QIcon>
#include <QIconEngine>
#include <QPixmap>
#include <QSize>

class QPainter;
class QRect;

enum class SageIconGlyph
{
    Close,
    Info,
    Warning,
    Error
};

class SageIconEngine : public QIconEngine
{
public:
    explicit SageIconEngine(SageIconGlyph glyph);

    void paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state) override;
    QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) override;
    QPixmap scaledPixmap(const QSize& size, QIcon::Mode mode, QIcon::State state, qreal scale) override;
    QIconEngine* clone() const override;

private:
    void paintClose(QPainter* painter, const QRect& rect, QIcon::Mode mode) const;
    void paintMessage(QPainter* painter, const QRect& rect) const;
    QColor messageColor() const;

private:
    SageIconGlyph m_glyph;
};

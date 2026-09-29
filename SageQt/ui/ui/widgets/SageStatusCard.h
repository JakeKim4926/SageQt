#pragma once

#include <QFontMetrics>
#include <QRect>
#include <QString>
#include <QWidget>

class QPainter;
class QPaintEvent;
class QProgressBar;
class QVBoxLayout;
class SageButton;

class SageStatusCard : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(SageStatusCardVariant variant READ variant NOTIFY variantChanged)

public:
    enum class SageStatusCardVariant
    {
        Idle,
        Running,
        Completed,
        Failed
    };
    Q_ENUM(SageStatusCardVariant)

    explicit SageStatusCard(QWidget* parent = nullptr);

    SageStatusCardVariant variant() const;
    QString message() const;
    QString detail() const;
    int progressPercent() const;

    void setIdle(const QString& message);
    void setRunning(const QString& message);
    void setProgressPercent(int percent);
    void setResult(bool success, const QString& message, const QString& detail);

signals:
    void variantChanged();
    void openFolderRequested();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void createWidgets();
    void createLayout();
    void connectSignals();
    void applyState(SageStatusCardVariant variant, const QString& message, const QString& detail);
    void drawPendingContent(QPainter& painter) const;
    void drawResultContent(QPainter& painter) const;
    void drawResultIcon(QPainter& painter, const QRect& iconRect) const;
    QColor surfaceColor() const;
    QColor borderColor() const;
    QColor accentColor() const;
    QColor messageColor() const;
    static QString elidedPath(const QString& path, const QFontMetrics& metrics, int width);

private:
    SageStatusCardVariant m_variant = SageStatusCardVariant::Idle;
    QString m_message;
    QString m_detail;
    QVBoxLayout* m_layout = nullptr;
    QProgressBar* m_progressBar = nullptr;
    SageButton* m_openFolderButton = nullptr;
};

#pragma once

#include <QApplication>
#include <QDialog>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>
#include <QTimer>

#include <functional>

class SageTestModalDriver : public QObject
{
    Q_OBJECT

public:
    using SageModalHandler = std::function<void(QDialog&)>;

    explicit SageTestModalDriver(SageModalHandler handler, QObject* parent = nullptr)
        : QObject(parent)
        , m_handler(std::move(handler))
    {
        m_timer.setInterval(SAGE_TEST_MODAL_POLL_MS);
        connect(&m_timer, &QTimer::timeout, this, &SageTestModalDriver::handleActiveModal);
        m_timer.start();
    }

    QStringList titles() const
    {
        return m_titles;
    }

private slots:
    void handleActiveModal()
    {
        QDialog* dialog = qobject_cast<QDialog*>(QApplication::activeModalWidget());
        if (dialog == nullptr || dialog == m_lastDialog) {
            return;
        }
        m_lastDialog = dialog;
        m_titles.append(dialog->windowTitle());
        m_handler(*dialog);
    }

private:
    static constexpr int SAGE_TEST_MODAL_POLL_MS = 10;

    SageModalHandler m_handler;
    QTimer m_timer;
    QPointer<QDialog> m_lastDialog;
    QStringList m_titles;
};

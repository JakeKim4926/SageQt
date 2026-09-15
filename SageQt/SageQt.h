#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SageQt.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SageQtClass; };
QT_END_NAMESPACE

class SageQt : public QMainWindow
{
    Q_OBJECT

public:
    SageQt(QWidget *parent = nullptr);
    ~SageQt();

private:
    Ui::SageQtClass *ui;
};


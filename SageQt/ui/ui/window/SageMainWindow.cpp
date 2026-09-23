#include "ui/window/SageMainWindow.h"

#include "SageDefine.h"

SageMainWindow::SageMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(SAGE_UI_MAIN_WINDOW_TITLE);
}

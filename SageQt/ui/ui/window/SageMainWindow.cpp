#include "ui/window/SageMainWindow.h"

#include "SageDefine.h"

SageMainWindow::SageMainWindow(QWidget* Parent)
    : QMainWindow(Parent)
{
    setWindowTitle(SAGE_UI_MAIN_WINDOW_TITLE);
}

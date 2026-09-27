#include "ui/window/SageMainWindow.h"

#include <QApplication>

#ifdef _WIN32
#endif
int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    SageMainWindow mainWindow;
    mainWindow.show();
    return application.exec();
}

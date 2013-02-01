#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("MapperTool");
    a.setOrganizationName("fieldOfView");
    a.setOrganizationDomain("fieldofview.com");

    MainWindow w;
    w.show();
    
    return a.exec();
}

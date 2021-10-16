#include "document.h"
#include "mainwindow.h"

#include <DApplication>
#include <DWidgetUtil>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("DMarkEd");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    DApplication a(argc, argv);
    a.setProductName("DMarked");
    a.setApplicationName("DMarkdowneditor");

    MainWindow window;
    window.show();
    Dtk::Widget::moveToCenter(&window);

    return a.exec();
}

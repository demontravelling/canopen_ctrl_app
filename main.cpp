#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ros::init(argc, argv, "navigation_app");
    Widget w;
    w.show();

    return a.exec();
}

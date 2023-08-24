#include "widget.h"
#include <QApplication>
#include <globalapplication.h>

int main(int argc, char *argv[])
{
    GlobalApplication a(argc, argv);
 //   QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}

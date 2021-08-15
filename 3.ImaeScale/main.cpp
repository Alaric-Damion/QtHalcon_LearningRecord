#include "browsepic.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BrowsePic w;
    w.show();

    return a.exec();
}

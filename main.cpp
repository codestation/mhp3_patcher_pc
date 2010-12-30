#include <QtGui/QApplication>
#include "patcher.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Patcher w;
    w.show();

    return a.exec();
}

#include "QGraphicsItemDemo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGraphicsItemDemo w;
    w.show();
    return a.exec();
}
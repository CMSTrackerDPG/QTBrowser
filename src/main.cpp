#include "qtbrowser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTBrowser w;
    w.show();

    return a.exec();
}

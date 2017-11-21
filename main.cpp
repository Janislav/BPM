#include "bpm.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BPM w;
    w.show();

    return a.exec();
}

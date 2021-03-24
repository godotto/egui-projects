#include "recipeplanner.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RecipePlanner w;
    w.show();
    return a.exec();
}

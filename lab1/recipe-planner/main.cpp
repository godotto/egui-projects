#include "recipeplanner.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // set English locale to avoid problems with decimal separator
    QLocale::setDefault(QLocale::English);

    QApplication a(argc, argv);
    RecipePlanner w;
    w.show();
    return a.exec();
}

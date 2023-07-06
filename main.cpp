
#include <QApplication>

#include"logging.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    logging g;
    g.show();
    return a.exec();
}

#include "mainmenu.h"

#include <QApplication>

/*! Default main.
 * Serves as the entrypoint for launching the application.
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainMenu w;
    w.show();
    return a.exec();
}

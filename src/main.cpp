#include "mainmenu.h"

#include <QApplication>

/*! \mainpage Evo Project
 * 
 * Этот проект реализует 4 игры, такие как:
 * 
 * - Mosaic
 * - Hangman
 * - Memory
 * - Quiz
 * 
 * Проект написан на C++ при помощи фреймворка <a href="https://doc.qt.io/qt-5/index.html" target="_blank">Qt5</a>.
 * 
 * 
 * 
 * Ссылки:
 * - <a href="https://github.com/Klaasibub/Evo" target="_blank">GitHub репозиторий.</a> 
 * - <a href="https://github.com/Klaasibub/Evo/releases" target="_blank">Актуальная версия установщика.</a>
 * - <a href="https://github.com/Klaasibub/Evo/issues" target="_blank">Сообщить об ошибках.</a>
 */ 

/*! Default main.
 * Служит входоной точкой для запуска приложения.
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainMenu w;
    w.show();
    return a.exec();
}

#include <QTest>
#include <QObject>
#include "main_gui_tests.h"
#include "utils_tests.h"
#include "quiz_tests.h"
#include "hangman_tests.h"
#include "mosaic_tests.h"
#include "memory_tests.h"


int main(int argc, char** argv)
{
    int status = 0;

    QObject *tests[]
    {
        new UtilsTests,
        new MainGuiTests,
        new QuizTests,
        new HangmanTests,
        new MosaicTests,
        new MemoryTests,
    };

    for(auto& test : tests)
        status |= QTest::qExec(test, argc, argv);

    for(auto test : tests)
        delete test;

    return status;
}


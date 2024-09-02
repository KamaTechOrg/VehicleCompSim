#include <QApplication>

#include "Editor.h"
#include "Gui.h"


int main(int argv, char* args[])
{
    /*
    * TODO: run here some code, that somehow listens to camera/sensors, and acts acording to the conditions.json
    * ConditionsMangaer manager;
    * manager.run();
    */

    QApplication app(argv, args);
   /* Window window;
    window.show();*/
    Gui gui;
    gui.show();
    return app.exec();
}

/*
[+] [dropdown] [condition] [value] // A
    [and]
[+]     [dropdown] [condition] [value] // B
        [or]
[+]     [dropdown] [condition] [value] // C
    [and]
    [dropdown] [condition] [value] // D

[then] [dropdown] [value]
*/

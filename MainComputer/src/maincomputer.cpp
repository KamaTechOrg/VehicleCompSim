#include "maincomputer.h"

MainComputer::MainComputer() {}

void MainComputer::openEditor()
{
    editor.show();
}

void MainComputer::run()
{
    manager.run();
}

void MainComputer::stop()
{
    manager.stop();
}

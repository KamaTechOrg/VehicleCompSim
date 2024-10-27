#ifndef MAINCOMPUTERITEM_EDITOR_H
#define MAINCOMPUTERITEM_EDITOR_H

#include "../maincomputeritem.h"
#include "../gui/editpanel.h"

#define pppppppppppppppppppp 878

class MainComputerItem::Editor : public EditPanel::Editor{
    MainComputerModel& model;

public:

    Editor(MainComputerModel* _model);
    void open();
    void close();
};
#endif // MAINCOMPUTERITEM_EDITOR_H

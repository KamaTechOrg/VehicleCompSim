#pragma once

#include "observer.h"

class ObserverConfig {
public:
    static Observer* getObserver() {
        return observer;
    }

    static void setObserver(Observer* obs) {
        observer = obs;
    }

private:
    static Observer* observer;
};

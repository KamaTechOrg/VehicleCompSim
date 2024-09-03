#pragma once

#include "canbus.h"

struct CompareCanBus {
    bool operator()(const CanBus& lhs, const CanBus& rhs) const {
        return lhs.getSourceId() > rhs.getSourceId();
    }
};
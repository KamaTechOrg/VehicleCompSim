#ifndef VEHICLE_COMPONENTS_ID_CONFIG_H
#define VEHICLE_COMPONENTS_ID_CONFIG_H

enum vehicle_components_id_config {
    MAIN_COMPUTER, // 0
    IMAGE_INPUT, // 1
    WHEELS, // 2
    BREAKS, // 3
    GAS, // 4
    COMPONENTS_COUNT
};

enum ACTIONS {
    PRESS_BREAKS,
    PRESS_GAS
};

#endif // VEHICLE_COMPONENTS_ID_CONFIG_H

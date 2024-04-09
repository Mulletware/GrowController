using namespace GrowController;

controllerSettings_t settings = {
    temp : {
        // { target, min, max }
        {30, 20, 32}, // day
        {20, 19, 22}  // night
    },
    vpd : {
        {1.0, 0.50, 1.05},
        {1.2, 1.1, 1.3}},
    watering : {50, 1000, 20000},
    lighting : {
        {0, 1, 0}, // on at 12:01
        {0, 0, 0}  // off at 12:00
    }
};
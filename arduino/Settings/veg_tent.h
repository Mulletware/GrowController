using namespace GrowController;

controllerSettings_t settings = {
    temp : {
        // { target, min, max }
        {28, 25, 28.5}, // day
        {20, 19, 22}    // night
    },
    vpd : {
        {0.7, 0.5, 1.3},
        {1.1, 1.0, 1.2}},
    watering : {50, 1000, 20000},
    lighting : {
        {0, 0, 10}, // on at 10:00 / 10 am
        {0, 0, 2}   // off at 2:00 / 2 am
    }
};
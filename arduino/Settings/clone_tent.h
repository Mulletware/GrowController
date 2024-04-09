using namespace GrowController;

controllerSettings_t settings = {
    temp : {
        // { target, min, max }
        {28, 25, 28.5}, // day
        {20, 19, 22}    // night
    },
    vpd : {
        {0.5, 0.4, 0.6},
        {1.2, 1.1, 1.3}},
    watering : {50, 1000, 20000},
    lighting : {
        {0, 10, 0}, // on at 8:00 / 8 am
        {0, 2, 0}   // off at 22:00 / 10 pm
    }
};
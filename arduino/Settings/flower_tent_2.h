// tent nearest electrical panel
using namespace GrowController;

controllerSettings_t settings = {
    temp : {
        // { target, min, max }
        {21, 20, 22}, // day
        {20, 19, 22}  // night
    },
    vpd : {
        {1.0, 0.90, 1.05},
        {1.2, 1.1, 1.3}},
    watering : {50, 1000, 20000},
    lighting : {
        {0, 1, 0}, // on at 8:00 / 8 am
        {0, 0, 0}  // off at 22:00 / 10 pm
    }
};
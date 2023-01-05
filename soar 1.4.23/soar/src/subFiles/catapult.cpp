#include "main.h"
#include "subHeaders/globals.hpp"

//helpers
void setCata(int pwr) {
    catapult = pwr;
}
// driver control funcs
void setCataMotor() {
    int power = 0;
    // bottom cata down, top cata up
    if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_R2) && limit.get_value()) {
        setCata(127);
    }
    if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        launch();
    }
}

void reload() {
    while(limit.get_value()) {
        setCata(127);
        pros::delay(2);
    }
    cece.rumble("-");
    setCata(0);
}

void launch() {
    if(!limit.get_value()) {
        catapult.tare_position();
        int target = 1000;
        int error = 0;
        cece.rumble(".");
        do {
            error = target*10 - catapult.get_position();
            catapult = error*20;
        } while (error > 0);
    cece.rumble(".");
    }
}
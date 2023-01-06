#include "main.h"

// driver control funcs
void setCataMotor(bool autoReload) {
    int power = 0;
    if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_R2) && (limit.get_value() == 0)) {
        power = 127;
    }
    if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && (limit.get_value() == 1)) {
        power = 70;
    }
    if(autoReload && (limit.get_value() == 0)) {
        power = 127;
    }
   catapult = power;
}

void reload() {
    while(limit.get_value() == 0) {
   catapult = 127;
    }
    cece.rumble("-");
   catapult = 0;
}

void launch() {
    cece.rumble(".");
    while((limit.get_value() == 1)) {
   catapult = 70;
    }
    cece.rumble(".");
   catapult = 0;
}
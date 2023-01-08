#include "main.h"
#include "pros/misc.h"
#include "subHeaders/globals.hpp"
#include <string>

// setMotors
void setCataMotor(bool autoReload, const char *launch, const char *reload) {
    int power = 0;

    //mapping
    auto launchVal = controllerMapping(launch, pros::E_CONTROLLER_DIGITAL_R1);
    auto reloadVal = controllerMapping(reload, pros::E_CONTROLLER_DIGITAL_R2);

    if(cece.get_digital(reloadVal) && (limit.get_value() == 0)) {
        power = 127;
    }
    if(cece.get_digital(launchVal) && (limit.get_value() == 1)) {
        power = 70;
    }
    if(autoReload && (limit.get_value() == 0)) {
        power = 127;
    }
   catapult = power;
}

void setIntakeMotors(const char *intaking, const char *outtaking){
    int intakePower = 0;

    //mapping
    auto intakeVal = controllerMapping(intaking, pros::E_CONTROLLER_DIGITAL_L1);
    auto outtakeVal = controllerMapping(outtaking, pros::E_CONTROLLER_DIGITAL_L2);

    // bottom outtake, top intake
    if(cece.get_digital(intakeVal) && limit.get_value() == 1){
        intakePower = 127;
    }
    else if(cece.get_digital(outtakeVal)){
        intakePower = -127;
    }

    intake = intakePower;
}

void setDriveMotors(){
    int power = cece.get_analog(ANALOG_LEFT_Y); // power of forward/backward movement
    int turn = cece.get_analog(ANALOG_RIGHT_X); // power of turn movement

    // dead zone (prevents stick drift)
    if(abs(power) < 5){
        power = 0;
    }
    if(abs(turn) < 5){
        turn = 0;
    }

    setDrive(power + 0.85*turn, power - 0.85*turn); // arcade drive
}

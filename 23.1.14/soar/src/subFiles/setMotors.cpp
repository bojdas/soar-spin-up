#include "main.h"
#include "pros/misc.h"
#include "subHeaders/globals.hpp"
#include "subHeaders/helpersAndExtras.hpp"

// setMotors
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

void setIntakeMotors(){
    int intakePower = 0;
    // bottom outtake, top intake
    if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_L1) && limit.get_value() == 1){
        intakePower = 127;
    }
    else if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
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

    setDrive(power + 0.75*turn, power - 0.75*turn); // arcade drive
}

void goofyControls(){
    int left = 0, right = 0, intakePowerGoof = 0;
    if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        left = 127;
    }
    if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        right = 127;
    }
    if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        right = -127;
    }
    if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        left = -127;
    }
    intakePowerGoof = cece.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
    if(abs(cece.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)) > 50 && limit.get_value() == 1) {
        catapult = 127;
    } else {
        catapult = 0;
    }
    if(abs(cece.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)) > 50 && limit.get_value() == 0) {
        catapult = 127;
    } else {
        catapult = 0;
    }
    setDrive(left, right);
}
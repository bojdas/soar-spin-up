#include "main.h" 
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "subHeaders/globals.hpp"
#include "subHeaders/setMotors.hpp"

int joystickMap[128] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
//helpers
int left = 0, right = 0;
void setDrive(int L, int R){
    if(L > 127) L = 127;
    if(R > 127) R = 127;
    if(L < -127) L = -127;
    if(R < -127) R = -127;

    left = L;
    right = R;
}

void resetEncoders(){
    lBack.tare_position();
    lFront.tare_position();
    lMid.tare_position();
    rMid.tare_position();
    rBack.tare_position();
    rFront.tare_position();
}

double avgEncoderValue(){
    return (fabs(lBack.get_position())
            + fabs(lFront.get_position()) 
            + fabs(lMid.get_position())
            + fabs(rMid.get_position())
            + fabs(rBack.get_position()) 
            + fabs(rFront.get_position())) / 6;
}

double trial(int i, bool side) {
    double rpm = 0;
    if(side) {
        leftDrive = i;
        pros::delay(2000);
        resetEncoders();
        pros::delay(5000);
        rpm = (((lFront.get_position() + lMid.get_position() + lBack.get_position())/3.0)/(900.0*0.5))*12.0;
        leftDrive = 0;
    } else {
        rightDrive = i;
        pros::delay(2000);
        resetEncoders();
        pros::delay(5000);
        rpm = (((rFront.get_position() + rMid.get_position() + rBack.get_position())/3.0)/(900.0*0.5))*12.0;
        rightDrive = 0;
    }
    pros::delay(3000);
    return rpm;
}

void Mapping(int i, bool side) {
    double t1 = 0, t2 = 0, t3 = 0;
    t1 = trial(i, side);
    cece.clear();
    cece.print(0, 0, "rpm: %f", t1);
    while (!cece.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
    }
    t2 = trial(i, side);
    cece.clear();
    cece.print(0, 0, "rpm: %f", t2);
    while (!cece.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
    }
    t3 = trial(i, side);
    double avg = (t1 + t2 +t3)/3.0;
    cece.print(0, 0, "rpm: %f", avg);
}


int slewRate() {
  int lError = 0, rError = 0;
  int placedLeft = 0, placedRight = 0;
  int prevLeft = 0, prevRight = 0;

  double kS = 0.35;
  while(true) {
        if(left == 0) {
            lError = left - prevLeft;
        } else if (left == 127) {
            lError = left + 2 - prevLeft;
        } else if (left == -127) {
            lError = left - 2 - prevLeft;
        } else {
            lError = left - prevLeft;
        }
        if(right == 0) {
            rError = right - prevRight;
        } else if (right == 127) {
            rError = right + 2 - prevRight;
        } else if (right == -127) {
            rError = right - 2 - prevRight;
        } else {
            rError = right - prevRight;
        }    
    placedLeft += lError * kS;
    placedRight += rError *kS;

    rightDrive = joystickMap[placedRight];
    leftDrive = joystickMap[placedLeft];

    prevLeft = placedLeft;
    prevRight = prevRight;
    pros::delay(25);
  }
}


pros::controller_digital_e_t controllerMapping(const char *function, pros::controller_digital_e_t usual) {
    auto control = usual;
            if(strcmp(function, "r1") == 0) control = pros::E_CONTROLLER_DIGITAL_R1;
            if(strcmp(function, "r2") == 0) control = pros::E_CONTROLLER_DIGITAL_R2;
            if(strcmp(function, "l1") == 0) control = pros::E_CONTROLLER_DIGITAL_L1;
            if(strcmp(function, "l2") == 0) control = pros::E_CONTROLLER_DIGITAL_L2;
            if(strcmp(function, "up") == 0) control = pros::E_CONTROLLER_DIGITAL_UP;
            if(strcmp(function, "down") == 0) control = pros::E_CONTROLLER_DIGITAL_DOWN;
            if(strcmp(function, "left") == 0) control = pros::E_CONTROLLER_DIGITAL_LEFT;
            if(strcmp(function, "right") == 0) control = pros::E_CONTROLLER_DIGITAL_RIGHT;
            if(strcmp(function, "x") == 0) control = pros::E_CONTROLLER_DIGITAL_X;
            if(strcmp(function, "y") == 0) control = pros::E_CONTROLLER_DIGITAL_Y;
            if(strcmp(function, "a") == 0) control = pros::E_CONTROLLER_DIGITAL_A;
            if(strcmp(function, "b") == 0) control = pros::E_CONTROLLER_DIGITAL_B;
    return control;
}
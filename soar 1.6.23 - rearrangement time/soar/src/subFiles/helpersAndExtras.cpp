#include "main.h" 
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
void setDrive(int left, int right){
    if(left > 127) left = 127;
    if(right > 127) right = 127;
    if(left < -127) left = -127;
    if(right < -127) right = -127;

    //left = joystickMap[left];
    //right = joystickMap[right];

    leftDrive = left;
    rightDrive = right;
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
        double rpm = (((lFront.get_position() + lMid.get_position() + lBack.get_position())/3.0)/(300.0*0.5))*12.0;
    } else {
       rightDrive = i;
        pros::delay(2000);
        resetEncoders();
        pros::delay(5000);
        double rpm = (((rFront.get_position() + rMid.get_position() + rBack.get_position())/3.0)/(300.0*0.5))*12.0;
    }
    pros::delay(3000);
    return rpm;
}

void Mapping(int i, bool side) {
    double t1 = 0, t2 = 0, t3 = 0;
    t1 = trial(i, side);
    while (cece.get_digital(E_CONTROLLER_DIGITAL_A)) {
    }
    t2 = trial(i, side);
    while (cece.get_digital(E_CONTROLLER_DIGITAL_A)) {
    }
    t3 = trial(i, side);
    double avg = (t1 + t2 +t3)/3.0;
    cece.print(0, 0, "rpm: %d", avg);
}

int slewRate() {
  int lError = 0, rError = 0, kS = 0.25, placedLeft = 0, placedRight = 0;
  while(true) {
    lError = left - (lBack + lMid + lFront)/3;
    rError = right - (rBack + rMid + rFront)/3;
    placedLeft += lError * kS;
    placedRight += rError *kS;

    rightDrive = placedRight;
    leftDrive = placedLeft;
    pros::delay(25);
  }
}

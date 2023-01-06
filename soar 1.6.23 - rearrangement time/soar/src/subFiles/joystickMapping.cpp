#include "main.h" 
#include "pros/rtos.hpp"
#include "subHeaders/globals.hpp"
#include "subHeaders/setMotors.hpp"

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
    t2 = trial(i, side);
    t3 = trial(i, side);
    double avg = (t1 + t2 +t3)/3.0;
    cece.print(0, 0, "rpm: %d", avg);
}
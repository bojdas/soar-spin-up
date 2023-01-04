#include "main.h"

// driver control funcs
void setIntakeMotors(){
    // bottom outtake, top intake
    if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
        intake = 127;
    }
    else if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        intake = -127;
    }
}

void setIntakeTime(int intPwr, int time){
    intake = intPwr;
    pros::delay(time);
    intake = 0;
}

void autoIntake(int dist, int volts){
    int direction = abs(dist) / dist;
    intake.tare_position();
    while(intake.get_position() < dist){
        intake = volts * direction;
    }
    intake = 0;
}
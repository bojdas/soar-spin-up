#include "main.h"

// helpers
void setIntake(int intPwr){
    intake = intPwr;
}

// driver control funcs
void setIntakeMotors(){
    int intakePower = 0;
    // bottom outtake, top intake
    if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
        intakePower = 127;
    }
    else if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        intakePower = -127;
    }

    setIntake(intakePower);
}

void setIntakeTime(int intPwr, int time){
    setIntake(intPwr);
    pros::delay(time);
    setIntake(0);
}

void autoIntake(int dist, int volts){
    int direction = abs(dist) / dist;
    intake.tare_position();
    while(intake.get_position() < dist){
        setIntake(volts * direction);
    }
    setIntake(0);
}
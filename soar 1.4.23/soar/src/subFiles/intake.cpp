#include "main.h"

//helper
void setIntake(int intPwr){
    intake = intPwr;
}

// driver control funcs
void setIntakeMotors(){
    int intakePower = 0;
    // bottom outtake, top intake
    if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_L1) && !limit.get_value()){
        intakePower = 127;
    }
    else if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        intakePower = -127;
    }

    setIntake(intakePower);
}

void intakeToggle(){
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
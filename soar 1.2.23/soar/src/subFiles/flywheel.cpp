#include "main.h"

// variables for TBH function
int target;
int current;
int output = 0;
double tbh = 0;
double gain = 0.4;
double error;
double pastError;


void takeBackHalf(int target){
    current = flywheel.get_actual_velocity();

    output = target;
    error = target - current; // calculate the error;
    output += gain * error + 8.71; // integrate the output;
    if (std::signbit(error)!= std::signbit(pastError)) { // if zero crossing,
        output = 0.5 * (output + tbh); // then Take Back Half
        tbh = output; // update Take Back Half variable
        pastError = error; // and save the previous error
    }

    flywheel.move_velocity(output);
}

// driver control funcs
void setFlywheelMotors(){
    // bottom to spin flywheel
    flywheel.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        takeBackHalf(425);
    }
    else{
        flywheel = 0;
    }
}

void setFlywheelandIndTime(int pwr, int indPwr){
    flywheel.move_velocity(pwr);
    pros::delay(3500);
    setIntakeTime(indPwr, 250);
    pros::delay(1400);
    setIntakeTime(indPwr, 400);
    pros::delay(700);
    flywheel = 0;
}
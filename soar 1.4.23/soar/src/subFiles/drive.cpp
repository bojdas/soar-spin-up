#include "main.h"

// helper funcs
void setDrive(int left, int right){
    if(left > 127) left = 127;
    if(right > 127) right = 127;
    if(left < -127) left = -127;
    if(right < -127) right = -127;

    lBack = left;
    lFront = left;
    lMid = left;
    rMid = right;
    rBack = right;
    rFront = right;
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

// driver control funcs
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

void setDriveTime(int left, int right, int time){
    setDrive(left, right);
    pros::delay(time);
    setDrive(0,0);
}
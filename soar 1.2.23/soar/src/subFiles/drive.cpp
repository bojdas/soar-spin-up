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

void translate(int units, int volts, int timeout){
    int direction = abs(units) / units;
    int initial = pros::millis();
    // float error = 0;
    
    // reset encoders + gyro
    resetEncoders();
    // gyro.tare();

    // go until units reached
    while(avgEncoderValue() < abs(units) && (pros::millis() - initial < timeout)){
        // error = gyro.get_heading();
        // if(error > 180){
        // error = error - 360;
        // }
        // set drive + self-correcting code according to gyro angle
        setDrive(volts * direction, volts * direction);
        pros::delay(10);
    }

    // brake
    setDrive(-10 * direction, -10 * direction);
    pros::delay(100);

    // drive back to neutral
    setDrive(0, 0);

    pros::delay(100);
}

void rotate(float degrees, int volts){
    int direction = fabs(degrees) / degrees;
    // reset gyro
    gyro.tare();

    // turn until degrees met
    setDrive(-volts * direction, volts * direction);
    while (fabs(gyro.get_rotation()) < fabs(degrees)){ // -3 prevents overshoot 
        pros::delay(10);
    }

    setDrive(-10*direction, 10*direction);
    pros::delay(100); // momentum check

    // // check overshoot
    // if(fabs(gyro.get_heading()) > abs(degrees)){
    //     setDrive(volts * direction * 0.5, -volts * direction * 0.5);
    //     while(fabs(gyro.get_rotation()) > abs(degrees)){
    //         pros::delay(10);
    //     }   
    // }

    // // check undershoot
    // if(fabs(gyro.get_heading()) < abs(degrees)){
    //     setDrive(-volts * direction * 0.4, volts * direction * 0.4);
    //     while(fabs(gyro.get_rotation()) > abs(degrees)){
    //         pros::delay(10);
    //     }   
    // }

    // reset drive
    setDrive(0, 0);

    pros::delay(100);
}
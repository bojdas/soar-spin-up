#include "ARMS/flags.h"
#include "main.h"
#include "ARMS/config.h"
#include "pros/motors.hpp"
#include <cmath>
#include <iostream>

pros::Controller master(CONTROLLER_MASTER);

//motor thingy
pros::Motor motor1(15, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor motor2(17, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor_Group motorGroup ({motor1, motor2});

//lefts
pros::Motor lFront(1, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lMid(2, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lBack(3, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);

//rights
pros::Motor rFront(10, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rMid(9, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rBack(8, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor_Group leftDrive ({lFront, lMid, lBack});
pros::Motor_Group rightDrive ({rFront, rMid, rBack});

pros::Imu gyro(16); //CHANGEA LTER
pros::ADIDigitalIn limit ('A');
pros::ADIDigitalOut endgame('C');
pros::ADIDigitalOut boosters('B');


//-----------------------AUTON FUNCTIONS-----------------------------------------
//-----------------------AUTON FUNCTIONS-----------------------------------------
//--------------------------------------------------------DIST PID----------------------------------------------------------
void distPID(int dist, int driveCAP, int timeout, double kP, double kD, int settleTime) {
    //transform shennanigans
    int start = pros::millis();
    double error = 0;
    double prevError = 0;
    double derivative = 0;
    double integral = 0;
    int settle = 0;
    settleTime = (int)settleTime/20;
    //note normal is kP = 0.3, kD = 2;
    if(kP == -1) kP = 0.3;
    if(kD == -1) kD = 2;

    double kI = 0, kO = 10;
    leftDrive.tare_position();
    rightDrive.tare_position();
    //gyro.tare_rotation();
    double initAngle = gyro.get_rotation();
    do{ //PID runs until within 1/4 inch
        error = dist * 45.4728408834 - (lFront.get_position()+lMid.get_position()+lBack.get_position()+rFront.get_position()+rMid.get_position()+rBack.get_position())/6; // 35.2589412388 for new drive
        derivative = error - prevError;
        integral += error;
        if ((error == 0)  || (error > dist) || integral > error)  integral = 0;
        double transform = error * kP + derivative * kD + integral * kI;
        double offset = (gyro.get_rotation() - initAngle) *kO;
        if(transform > driveCAP) transform = driveCAP;
        if(transform < -driveCAP) transform = -driveCAP;
        if(transform < 15 && transform > 0) {
            transform = 15;
            offset = 0;
        }
        if(transform > -15 && transform < 0) {
            transform = -15;
            offset = 0;
        }
        //set drive
        leftDrive = (int)transform;
        rightDrive = (int)transform;
        //setting reload for cata
        if(limit.get_value() == 0) {
            motorGroup = 127;
        } else {
            motorGroup = 0;
        }

        //setting prev errors
        prevError = error;
        //timeout
        if(pros::millis()-start > timeout) {
            break;
        }
        pros::delay(20);
        if(fabs(error) < 45.4728408834/2) { 
            settle++;
        }
    }while(settle < settleTime);
    leftDrive = 0;
    rightDrive = 0;
    motorGroup = 0;
}

void distPID(int dist, int driveCAP, int timeout, double kP, double kD) {
    distPID(dist, driveCAP, timeout, kP, kD, 140);
}
//--------------------------------------------------------TURN PID----------------------------------------------------------
void turnPID(int angle, int driveCAP, int timeout, double kP, double kD, bool relative, int settleTime) {
    //transform shennanigans
    int start = pros::millis();
    double error = 0;
    double prevError = 0;
    double derivative = 0;
    double integral = 0;
    int settle = 0;

    settleTime = (int)settleTime/20;
    // note: kP = 1.5, kD = 4 by default
    if(kP == -1) kP = 1.5;
    if(kD == -1) kD = 4;
    double kI = 0;

    if(!relative) {
        angle = angle - (int)gyro.get_rotation()%360;
        if (angle > 180)
            angle -= 360;
        else if (angle < -180)
            angle += 360;
    }

    do{ //PID runs until within 2 degrees
        error = angle - (int)gyro.get_rotation()%360;
        //printf("turn error: %i", (int)error);
        derivative = error - prevError;
        integral += error;
        if ((error == 0)  || (error > angle))  integral = 0;
        double rotate = error * kP + derivative * kD + integral * kI;
        if(rotate > driveCAP) rotate = driveCAP;
        if(rotate < -driveCAP) rotate = -driveCAP;
        if(rotate < 20 && rotate > 0) rotate = 20;
        if(rotate > -20 && rotate < 0) rotate = -20;
        //set drive
        leftDrive = (int)rotate;
        rightDrive = (int)-rotate;

        //setting reload for cata
        if(limit.get_value() == 0) {
            motorGroup = 127;
        } else {
            motorGroup = 0;
        }

        //setting prev errors
        prevError = error;
        //timeout
        if(pros::millis()-start > timeout) {
            break;
        }
        pros::delay(20);
        if(fabs(error) == 0) {
            settle++;
        }
    }while(settle < settleTime);
    leftDrive = 0;
    rightDrive = 0;
    motorGroup = 0;
}

void turnPID(int angle, int driveCAP, int timeout) {
    turnPID(angle, driveCAP, timeout, -1, -1, true, 120);
}

void turnPID(int angle, int timeout) {
    turnPID(angle, 120, timeout, -1, -1, true, 120);
}

void turnPID(int angle, int driveCAP, int timeout, bool relative, int settleTime) {
    turnPID(angle, driveCAP, timeout, -1, -1, relative, settleTime);
}

void rollerPID(double rotation, int timeout) {
    int start = pros::millis();
    double error = 0;

    double kP=2;
    double exitOffset = 100;
    motor1.tare_position();

    do{ //PID runs until within a good bit
        error = rotation * 7200 - motor1.get_position();
        double rotatePower = error * kP;

        if(rotatePower < -127) rotatePower = -127;
        if(rotatePower > 0) break;
        if(rotatePower > -15 && rotatePower < 0) rotatePower = -15;
        //set motorGroup
        motorGroup = rotatePower;

        //setting reload for cata
        if(limit.get_value() == 0) {
            motorGroup = 127;
        } else {
            motorGroup = 0;
        }

        //timeout
        if(pros::millis()-start > timeout) {
            break;
        }
        pros::delay(5);
    }while(fabs(error) > 1500.0/16);
    motorGroup = 0;
}

void reload() {
    while(limit.get_value() == 0) {
   motorGroup = 127;
    }
    master.rumble("-");
   motorGroup = 0;
}

void launch() {
    master.rumble(".");
    while((limit.get_value() == 1)) {
   motorGroup = 127;
    }
    master.rumble(".");
   motorGroup = 0;
}

void rightAuto(){
    //-------------ROLLER + 2 LOW GOAL-----------
    using namespace arms::chassis;
    move(16.8, 100, 2000);
    turn(90, 100, arms::RELATIVE);
    move(-3.6, 100, arms::REVERSE);
    rollerPID(-0.15, 1000);
    move(3.6, 100);
    turn(95,100, arms::RELATIVE);
    launch();
    turn(135,100, arms::RELATIVE);
    reload();
    motorGroup = -127;
    move(-57.84,50,arms::REVERSE);
    turn(90, 100, arms::RELATIVE);
    move(4.8,100);
    pros::delay(1500);
    motorGroup = 0;
    reload();
    pros::delay(3000);
    launch();
}

void leftAuto(){
    //--------------roller + 2 low goal + line up for 1/2 high goal-------------
    using namespace arms::chassis;
    move(-3.6, 100, arms::REVERSE);
    rollerPID(-0.2, 1000);
    move(3.6,100);
    turn(-90,100, arms::RELATIVE);
    launch();
    turn(-135,100, arms::RELATIVE);
    reload();
    move(-24.96,100);
    motorGroup = -127;
    move(-25.92,60);
    turn(-96,100, arms::RELATIVE);
    move(9.6,100);
    pros::delay(1000);
    motorGroup = 0;
    pros::delay(3000);
    launch();
    reload();
}

void skillsPath() { 
	using namespace arms::chassis;
    //----------------------------2 ROLLERS + PRELOAD SHOT--------------------------------
    pros::delay(500);
    move(-4,1);
    rollerPID(0.3, 1000);
    move(4,1);
    turn(60,70,2);
    move(24, 55,3);
    turn(280,70,2);
    move(-12,80,2, arms::REVERSE);
    pros::delay(500);
    rollerPID(0.3, 1000);
    move(5,2);
    turn(0,1);
    move(67,5);
    pros::delay(1000);

    //----------------------------LOWGOAL SHOT--------------------------------
    turn(-89,80,2);
    move(28,50,5);
    move(-28,4,arms::REVERSE);
    turn(-2,2);
    pros::delay(1000);

    //----------------------------RIGHT SIDE CORNER SHOT--------------------------------
    turn(-145,70,2);
    move(40,60,3);
    turn(-44,90,1);
    move(38,50,3);
    turn(46,2);
    move(12,2);
    pros::delay(1000);

    //----------------------------1 ROLLER + 3 STACK LEFT SIDE SHOT--------------------------------
    move(-12,2,arms::REVERSE);
    turn(-45,2);
    move(63,60,5);
    turn(180,2);
    move(-12,70,5,arms::REVERSE);
    pros::delay(750);
    rollerPID(0.3, 2000);
    move(9,2);
    turn(89,1);
    move(47,5);
    pros::delay(1000);

    //----------------------------1 ROLLER + 3 STACK MIDLINE SHOT--------------------------------
    turn(-117,2);
    move(40,80,6);
    turn(-87,70,2);
    move(24,60,2);
    turn(90,70,2);
    move(-24,60,5,arms::REVERSE);
    rollerPID(0.3, 2000);
    move(5,2);
    turn(180,2);
    move(52,5);
    pros::delay(1000);
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



void skills() { 
	using namespace arms::chassis;
    //----------------------------2 ROLLERS + PRELOAD SHOT--------------------------------
    pros::delay(500);
    move(-4,1);
    rollerPID(0.3, 1000);
    move(4,1);
    turn(60,70,2);
    reload();
    motorGroup = -127;
    move(24, 55,3);
    turn(280,70,2);
    motorGroup = 0;
    move(-12,80,2, arms::REVERSE);
    pros::delay(500);
    rollerPID(0.3, 1000);
    move(5,2);
    turn(0,1);
    move(67,5);
    pros::delay(1000);
    launch();

    //----------------------------LOWGOAL SHOT--------------------------------
    turn(-89,80,2);
    reload();
    motorGroup = -127;
    move(28,50,5);
    move(-28,4,arms::REVERSE);
    turn(-2,2);
    motorGroup = 0;
    pros::delay(1000);
    launch();

    //----------------------------RIGHT SIDE CORNER SHOT--------------------------------
    turn(-145,70,2);
    reload();
    motorGroup = -127;
    move(40,60,3);
    turn(-44,90,1);
    move(38,50,3);
    turn(46,2);
    move(12,2);
    motorGroup = 0;
    pros::delay(1000);
   launch();

    //----------------------------1 ROLLER + 3 STACK LEFT SIDE SHOT--------------------------------
    move(-12,2,arms::REVERSE);
    turn(-45,2);
    reload();
    motorGroup = -127;
    move(63,60,5);
    turn(180,2);
    motorGroup = 0;
    move(-12,70,5,arms::REVERSE);
    pros::delay(750);
    rollerPID(0.3, 2000);
    move(9,2);
    turn(89,1);
    move(47,5);
    pros::delay(1000);
    launch();

    //----------------------------1 ROLLER + 3 STACK MIDLINE SHOT--------------------------------
    turn(-117,2);
    move(40,80,6);
    reload();
    motorGroup = -127;
    turn(-87,70,2);
    move(24,60,2);
    turn(90,70,2);
    motorGroup = 0;
    move(-24,60,5,arms::REVERSE);
    rollerPID(0.3, 2000);
    move(5,2);
    turn(180,2);
    move(52,5);
    pros::delay(1000);
    launch();
}
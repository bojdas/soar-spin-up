#include "main.h"
#include "pros/rtos.hpp"
#include "subHeaders/autons.hpp"
#include "subHeaders/globals.hpp"
#include "subHeaders/helpersAndExtras.hpp"
#include <cmath>
#include <iostream>

//note: one inch for drive is equal to 52.0870722846 encoder ticks

//-----------------------AUTON FUNCTIONS-----------------------------------------
//--------------------------------------------------------DIST PID----------------------------------------------------------
void distPID(int dist, int driveCAP, int timeout, double kP, double kD) {
    //transform shennanigans
    int start = pros::millis();
    double error = 0;
    double prevError = 0;
    double derivative = 0;
    double integral = 0;
    int settle = 0;

    //note normal is kP = 0.3, kD = 2;
    if(kP == -1) kP = 0.3;
    if(kD == -1) kD = 2;

    double kI = 0, kO = 30;
    //double exitOffset = 100;
    resetEncoders();
    gyro.tare_rotation();
    double initAngle = gyro.get_rotation();
    do{ //PID runs until within 1/4 inch
        error = dist * 12.6008973483 - avgEncoderValue(); // 35.2589412388 for new drive
      //  std::cout << "dist error: " << error << "\n";
      //  std::cout << "\n";
        derivative = error - prevError;
        integral += error;
        if ((error == 0)  || (error > dist) || integral > error)  integral = 0;
        double transform = error * kP + derivative * kD + integral * kI;
        std::cout << "dist error: " << (int)error << "\n";
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
        setDrive((int)transform-offset, (int)transform+offset);

        //setting reload for cata
        if(limit.get_value() == 0) {
            catapult = 127;
        } else {
            catapult = 0;
        }

        //setting prev errors
        prevError = error;
        //timeout
        if(pros::millis()-start > timeout) {
            break;
        }
        pros::delay(20);
        if(fabs(error) > 52.087/4) {
            settle++;
        }
    }while(fabs(error) > 52.0870722846/4); // 146.916666667 for new bot
    setDrive(0, 0);
    catapult = 0;
}

//--------------------------------------------------------TURN PID----------------------------------------------------------
void turnPID(int angle, int driveCAP, int timeout, double kP, double kD) {
    //transform shennanigans
    int start = pros::millis();
    double error = 0;
    double prevError = 0;
    double derivative = 0;
    double integral = 0;
    int settle = 0;
    // note: kP = 1.5, kD = 4 by default
    if(kP == -1) kP = 1.5;
    if(kD == -1) kD = 4;
    double kI = 0;
    //double exitOffset = 100;
    gyro.tare_rotation();

    if(angle != 0)
        angle = angle + gyro.get_rotation();

    do{ //PID runs until within 2 degrees
        error = angle - gyro.get_rotation();
        //printf("turn error: %i", (int)error);
       // std::cout << "turn error: " << error << "\n";
        derivative = error - prevError;
        integral += error;
        if ((error == 0)  || (error > angle))  integral = 0;
        double rotate = error * kP + derivative * kD + integral * kI;
        std::cout << "turn  error: " << (int) error << "\n";
        if(rotate > driveCAP) rotate = driveCAP;
        if(rotate < -driveCAP) rotate = -driveCAP;
        if(rotate < 20 && rotate > 0) rotate = 20;
        if(rotate > -20 && rotate < 0) rotate = -20;
        //set drive
        setDrive((int)rotate, (int)-rotate);

        //setting reload for cata
        if(limit.get_value() == 0) {
            catapult = 127;
        } else {
            catapult = 0;
        }

        //setting prev errors
        prevError = error;
        //timeout
        if(pros::millis()-start > timeout) {
            break;
        }
        pros::delay(20);
        if(fabs(error) > 0.5) {
            settle++;
        }
    }while(fabs(error) > 0.5);
    setDrive(0, 0);
    catapult = 0;
}

void rollerPID(double rotation, int timeout) {
    //transform shennanigans
    int start = pros::millis();
    double error = 0;

    double kP=2;
    double exitOffset = 100;
    intake.tare_position();

    do{ //PID runs until within a good bit
        error = rotation * 1500 - intake.get_position();
        double rotatePower = error * kP;

        if(rotatePower > 127) rotatePower = 127;
        if(rotatePower < -127) rotatePower = -127;
        if(rotatePower < 15 && rotatePower > 0) rotatePower = 15;
        if(rotatePower > -15 && rotatePower < 0) rotatePower = -15;
        //set intake
        intake = rotatePower;

        //setting reload for cata
        if(limit.get_value() == 0) {
            catapult = 127;
        } else {
            catapult = 0;
        }

        //timeout
        if(pros::millis()-start > timeout) {
            break;
        }
        pros::delay(5);
    }while(fabs(error) > 1500.0/16);
    intake = 0;
    catapult = 0;
}

void reload() {
    while(limit.get_value() == 0) {
   catapult = 127;
    }
    cece.rumble("-");
   catapult = 0;
}
void launch() {
    cece.rumble(".");
    while((limit.get_value() == 1)) {
   catapult = 127;
    }
    cece.rumble(".");
   catapult = 0;
}

//-----------------------AUTON PATHS-----------------------------------------

void rightAuto(){
    //--------------------------229v ACE ROBOTICS----------------------
    /*distPID(102, 100, 4500);
    pros::delay(200);
    //angles
    //launch();
    turnPID(-62, 127);
    intake = -127;
    distPID(-90, 120, 4500);
    distPID(-35, 40, 4500);
    pros::delay(125);
    intake = 0;
    distPID(10, 90, 4500);
    turnPID(155, 90);
    //intake = 127;
    distPID(-151, 80);
    distPID(-10, 40);
    distPID(10, 120);
    //intake = 0;
    turnPID(-100, 127); // dunno angle
    intake = 127; //ensuring discs are in
    pros::delay(500);
    intake = 0;
    //launch();
    pros::delay(100);
    turnPID(105, 127); // dunno angle
    reload();
    intake = 127;
    distPID(-121, 127);
    distPID(-20, 80);
    distPID(20, 120);
    turnPID(-90, 120);
    distPID(-10, 127);
    //launch();
    intake = 0;
    */

    //-------------ROLLER + 2 LOW GOAL-----------
    distPID(70, 120, 2000, -1, -1);
    turnPID(-90, 120, 1500, -1, -1);
    distPID(-15, 100, 1000, -1, -1);
    rollerPID(-0.15, 1000);
    distPID(15, 120, 1400, -1, -1);
    turnPID(-95, 120, 2000, -1, -1);
    launch();
    turnPID(-135, 120, 1000, -1, -1);
    reload();
    intake = 120;
    distPID(-241, 90, 3000, -1, -1);
    turnPID(-90, 120, 1000, -1, -1);
    distPID(20, 120, 1000, -1, -1);
    pros::delay(1500);
    intake = 0;
    reload();
    
    std::cout << "end of right auton reached";
}

void leftAuto(){
    //--------------roller + 2 low goal + line up for 1/2 high goal-------------
    distPID(-15, 100, 1000, -1, -1);
    rollerPID(-0.2, 1000);
    distPID(15, 100, 4500, -1, -1);
    turnPID(90, 120, 2000, -1, -1);
    launch();
    turnPID(135, 120, 1000, -1, -1);
    reload();
    distPID(-104, 120, 3000, -1, -1);
    /*intake = 127;
    distPID(-30, 60, 2000);
    pros::delay(1000);
    intake = -127;
    distPID(-78, 100, 2000);
    intake = 127;
    turnPID(96, 120, 1000);
    distPID(40, 100, 1000);
    pros::delay(1000);
    intake = 0;
    pros::delay(5000);
    launch();*/
    reload();
}

void newSkills() {
            distPID(-10, 100, 1000, -1, -1);
            rollerPID(0.33, 1000);
            distPID(20, 100, 4500, -1, -1);
            turnPID(120, 120, 2000, -1, -1);
            intake = 127;
            distPID(-80, 60, 4500, -1, -1);
            intake = 0;
            turnPID(-30, 100, 2000, -1, -1);
            distPID(-35, 100, 1000, -1, -1);
            rollerPID(0.4, 1000);
            distPID(15, 70, 1000, -1, -1);
            pros::delay(500);
            turnPID(270, 120, 4000, -1, -1);
            pros::delay(200);
            distPID(220, 120, 3000, -1, -1);
            turnPID(10, 100, 1000, -1, -1);
            launch();
            turnPID(-100, 70, 3000, -1, -1);
            reload();
            intake = 127;
            distPID(-110, 70, 3000, -1, -1);
            distPID(110, 120, 3000, -1, -1);
            intake = 0;
}
/*
void skills(){
//-------------------------FIRST 2 ROLLERS AND ONE SHOT---------------------------------------------
    drivePID(-5,0,127);
    //ROLLER
    drivePID(80,90,127); //dunno transform
    reload();
    intake = 127;
    drivePID(-250,0,60);
    intake = 0;

    //ROLLER
    drivePID(15,0,127);
    drivePID(0,-90,127);

    drivePID(300,0,127);
    drivePID(0,0,127); //dunno angle
    launch();
    drivePID(0,0,127); //dunno angle


//----------------------pick up lowgoal barrier discs & shoot---------------------------------
    drivePID(0,-90,127);
    reload();
    intake = 127;
    drivePID(-110,0,60);
     intake = 0;
   drivePID(110,0,127);
    drivePID(0,0,127); //dunno angle
    launch();
    drivePID(0,0,127); //dunno angle

//---------------pick up 1 disc and line up for other two--------------------------
    drivePID(0,0,127); //dunno angle
    reload();
    drivePID(-150,0,127); //dunno dist
    intake = 127;
    drivePID(-50,0,60); //dunno dist
    drivePID(25,0,127);
    intake = 0;
    drivePID(0,0,127); //dunno angle

//---------------------pick up other 2 discs & shoot by lowgoal corner---------------------------------
    intake = 127;
    drivePID(-161,0,60); //141 is abt sqrt(2);
    drivePID(20,0,127);
     intake = 0;
   drivePID(0,90,127);
    drivePID(0,0,127); //dunno dist
    launch();
    drivePID(-0,0,127); //dunno dist
    drivePID(0,-90,127);
  
  //-------------------------------getting 3 stack, shooting, & aligning w roller---------------------
    drivePID(-121,0,127); //this line and the following add up to abt sqrt(2)
    reload();
    intake = 127;
    drivePID(-161,0,50);
    drivePID(0,45,127);
    intake = 0;
    drivePID(250,0,127);
    launch();
    drivePID(-250,0,127); 
    drivePID(0,-90,127);
}

void skillsRoutingTest(){
//-------------------------FIRST 2 ROLLERS AND ONE SHOT---------------------------------------------
    drivePID(-5,0,127);
    //ROLLER
    drivePID(80,90,127); //dunno transform
    drivePID(-250,0,60);

    //ROLLER
    drivePID(15,0,127);
    drivePID(0,-90,127);

    drivePID(300,0,127);
    drivePID(0,0,127); //dunno angle
    //shoot
    drivePID(0,0,127); //dunno angle


//----------------------pick up lowgoal barrier discs & shoot---------------------------------
    drivePID(0,-90,127);
    drivePID(-110,0,60);
    drivePID(110,0,127);
    drivePID(0,0,127); //dunno angle
    //shoot
    drivePID(0,0,127); //dunno angle

//---------------pick up 1 disc and line up for other two--------------------------
    drivePID(0,0,127); //dunno angle
    drivePID(-150,0,127); //dunno dist
    drivePID(-50,0,60); //dunno dist
    drivePID(25,0,127);
    drivePID(0,0,127); //dunno angle

//---------------------pick up other 2 discs & shoot by lowgoal corner---------------------------------
    drivePID(-161,0,60); //141 is abt sqrt(2);
    drivePID(20,0,127);
    drivePID(0,90,127);
    drivePID(0,0,127); //dunno dist
    //shoot
    drivePID(-0,0,127); //dunno dist
    drivePID(0,-90,127);
  
  //-------------------------------getting 3 stack, shooting, & aligning w roller---------------------
    drivePID(-121,0,127); //this line and the following add up to abt sqrt(2)
    drivePID(-161,0,50);
    drivePID(0,45,127);
    drivePID(250,0,127);
    //shoot
    drivePID(-250,0,127); 
    drivePID(0,-90,127);
}*/
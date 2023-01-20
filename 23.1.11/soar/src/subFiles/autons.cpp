#include "main.h"
#include "pros/rtos.hpp"
#include "subHeaders/globals.hpp"
#include "subHeaders/helpersAndExtras.hpp"
#include <cmath>

//note: one inch for drive is equal to 52.0870722846 encoder ticks

//-----------------------AUTON FUNCTIONS-----------------------------------------
void distPID(int dist, int driveCAP) {
    //transform shennanigans
    int start = pros::millis();
    double error = 0;
    double prevError = 0;
    double derivative = 0;
    double integral = 0;
    double exitOffset = 200;

    double kP=0.3, kI=0, kD=1;
    resetEncoders();

    do{ //PID runs until within 1 degree and within 1/4 inch
        //transform
        error = dist * 12.6008973483 - avgEncoderValue();
        derivative = error - prevError;
        integral += error;
        if ((error == 0)  || (error > dist))  integral = 0;
        double transform =error * kP + derivative * kD + integral * kI;

        cece.print(0, 0, "dist error: %f", error);

        if(transform > driveCAP) transform = driveCAP;
        if(transform < -driveCAP) transform = -driveCAP;
        if(transform < 15 && transform > 2) transform = 15;
        if(transform > -15 && transform < - 2) transform = 15;
         setDrive((int)transform, (int)transform);

        //setting reload for cata
        if(limit.get_value() == 0) {
            catapult = 127;
        } else {
            catapult = 0;
        }

        //setting intake AND roller (mostly used for intake)
        intake = autonIntakePower;

        //setting prev errors
        prevError = error;

        //timeout
        if(pros::millis()-start > 6500) {
            break;
        }
        pros::delay(20);
        if(pros::millis()%1000==0) {
            exitOffset = error;
        }
    }while(fabs(exitOffset) > 52.0870722846/4);
    cece.rumble(".");
    setDrive(0, 0);
    catapult = 0;
}

void turnPID(int angle, int driveCAP) {
    //transform shennanigans
    int start = pros::millis();
    double error = 0;
    double prevError = 0;
    double derivative = 0;
    double integral = 0;
    double exitOffset = 200;

    double kP= 2, kI=0, kD=7;
    gyro.tare_rotation();
    
    if(angle != 0) {
        angle = (int)((double) angle * 0.98);
        angle += gyro.get_rotation();
    }
    do{ //PID runs until within 1 degree and within 1/4 inch
        //transform
        error = angle  - gyro.get_heading();
        derivative = error - prevError;
        integral += error;
        if ((error == 0)  || (error > angle))  integral = 0;
        double rotate =error * kP + derivative * kD + integral * kI;

        cece.print(0, 0, "angle error: %f", error);

        if(rotate > driveCAP) rotate = driveCAP;
        if(rotate < -driveCAP) rotate = -driveCAP;
        if(rotate < 15 && rotate > 2) rotate = 15;
        if(rotate > -15 && rotate < - 2) rotate = 15;

         setDrive((int)rotate, (int)rotate);

        //setting reload for cata
        if(limit.get_value() == 0) {
            catapult = 127;
        } else {
            catapult = 0;
        }

        //setting intake AND roller (mostly used for intake)
        intake = autonIntakePower;

        //setting prev errors
        prevError = error;

        //timeout
        if(pros::millis()-start > 6500) {
            break;
        }
        pros::delay(20);
        if(pros::millis()%1000==0) {
            exitOffset = error;
        }
    }while(fabs(exitOffset) > 1);
    cece.rumble(".");
    setDrive(0, 0);
    catapult = 0;
}

void drivePID(int dist, int angle, int driveCAP) {
    //transform shennanigans
    int start = pros::millis();
    double tError = 0;
    double tPrevError = 0;
    double tDerivative = 0;
    double tIntegral = 0;

    //goofy rotate
    double  rError = 0;
    double rPrevError = 0;
    double  rDerivative = 0;
    double rIntegral = 0;

    double tkP=0.3, tkI=0, tkD=1, rkP=3, rkI=0, rkD=7;
    double rExitOffset = 100, tExitOffset = 100;
    gyro.tare_rotation();
    resetEncoders();


    if(angle != 0) {
        angle = (int)((double) angle * 0.98);
        angle += gyro.get_rotation();
    }
    do{ //PID runs until within 1 degree and within 1/4 inch
        //transform
        tError = dist * 12.6008973483 - avgEncoderValue();
        tDerivative = tError - tPrevError;
        tIntegral += tError;
        if ((tError == 0)  || (tError > dist))  tIntegral = 0;
        double transform = tError * tkP + tDerivative * tkD + tIntegral * tkI;

        //rotate
        rError =  angle - gyro.get_rotation();
        rDerivative = rError - rPrevError;
       // rIntegral += rError;
        //if ((rError == 0)  || (rError > angle))
           // rIntegral = 0;
        double rotate = rError * rkP + rDerivative * rkD;
    cece.print(0, 0, "tError: %f", tError);

        // rIntegral * rkI;
        if(rotate > 100) rotate = 100;
        if(rotate < -100) rotate = -100;
        if(transform > driveCAP) transform = driveCAP;
        if(transform < -driveCAP) transform = -driveCAP;

        //setting drive
        if(dist == 0) {
            setDrive(rotate, -rotate);
        } else if (angle == 0){
             setDrive((int)transform, (int)transform);
        } else {
        setDrive(transform + rotate, transform - rotate);
        }

        //setting reload for cata
        if(limit.get_value() == 0) {
            catapult = 127;
        } else {
            catapult = 0;
        }

        //setting intake AND roller (mostly used for intake)
        intake = autonIntakePower;

        //setting prev errors
        tPrevError = tError;
        rPrevError = rError;

        //timeout
        if(pros::millis()-start > 6500) {
            break;
        }
        pros::delay(20);
        if(pros::millis()%1000==0) {
            rExitOffset = rError;
            tExitOffset = tError;
        }
    }while(fabs(rExitOffset) > 1 || fabs(tExitOffset) > 52.0870722846/4);

    cece.rumble(".");
    cece.print(0, 0, "rError: %f", rError);
    setDrive(0, 0);
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

void leftAuto(){
    drivePID(100, 0, 127);
    cece.rumble("---");
}

void rightAuto(){

}

void skills(){
//-------------------------FIRST 2 ROLLERS AND ONE SHOT---------------------------------------------
    drivePID(-5,0,127);
    //ROLLER
    drivePID(80,90,127); //dunno transform
    reload();
    autonIntakePower = 127;
    drivePID(-250,0,60);
    autonIntakePower = 0;

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
    autonIntakePower = 127;
    drivePID(-110,0,60);
     autonIntakePower = 0;
   drivePID(110,0,127);
    drivePID(0,0,127); //dunno angle
    launch();
    drivePID(0,0,127); //dunno angle

//---------------pick up 1 disc and line up for other two--------------------------
    drivePID(0,0,127); //dunno angle
    reload();
    drivePID(-150,0,127); //dunno dist
    autonIntakePower = 127;
    drivePID(-50,0,60); //dunno dist
    drivePID(25,0,127);
    autonIntakePower = 0;
    drivePID(0,0,127); //dunno angle

//---------------------pick up other 2 discs & shoot by lowgoal corner---------------------------------
    autonIntakePower = 127;
    drivePID(-161,0,60); //141 is abt sqrt(2);
    drivePID(20,0,127);
     autonIntakePower = 0;
   drivePID(0,90,127);
    drivePID(0,0,127); //dunno dist
    launch();
    drivePID(-0,0,127); //dunno dist
    drivePID(0,-90,127);
  
  //-------------------------------getting 3 stack, shooting, & aligning w roller---------------------
    drivePID(-121,0,127); //this line and the following add up to abt sqrt(2)
    reload();
    autonIntakePower = 127;
    drivePID(-161,0,50);
    drivePID(0,45,127);
    autonIntakePower = 0;
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
}
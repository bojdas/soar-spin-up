#include "main.h"
#include "pros/rtos.hpp"
#include "subHeaders/drive.hpp"
#include "subHeaders/flywheel.hpp"
#include "subHeaders/globals.hpp"
#include "subHeaders/intake.hpp"

bool activePID = true;
//silly tuners
double tkP=0.3, tkI=0, tkD=1, rkP=1, rkI=0, rkD=0;


void drivePID(int dist, int angle, int driveCAP) {
    //transform shennanigans
    int tError = 0;
    int tPrevError = 0;
    int tDerivative = 0;
    int tIntegral = 0;

    //goofy rotate
    int rError = 0;
    int rPrevError = 0;
    int rDerivative = 0;
    int rIntegral = 0;
    gyro.tare();
    resetEncoders();
    while((gyro.get_rotation() < angle - 1 || gyro.get_rotation() > angle + 1) &&
    (avgEncoderValue() < dist - 12 || avgEncoderValue() > dist + 12)){
        //transform
        tError = dist * 12.1 - avgEncoderValue();
        tDerivative = tError - tPrevError;
        tIntegral += tError;
        if ((tError == 0)  || (tError > dist))  tIntegral = 0;
        double transform = tError * tkP + tDerivative * tkD + tIntegral * tkI;

        //rotate
        rError =  angle - gyro.get_rotation();
        //rDerivative = rError - rPrevError;
       // rIntegral += rError;
        //if ((rError == 0)  || (rError > angle))
           // rIntegral = 0;
        double rotate = rError * rkP;
        // rDerivative * rkD;
        // rIntegral * rkI;
        if(rotate > 127) rotate = 127;
        if(rotate < -127) rotate = -127;
        if(transform > driveCAP) transform = driveCAP;
        if(transform < -driveCAP) transform = -driveCAP;

        //setting drive
        if(dist == 0) {
            setDrive(rotate, -rotate);
        } else if(angle == 0) {
            setDrive(transform, transform);
        } else {
        setDrive((int)transform + (int)rotate, (int)transform - (int) rotate);
        }

        //setting reload for cata
        if(!limit.get_value()) catapult = 127;

        //setting intake AND roller
        intake = autonIntakePower;

        //setting prev errors
        tPrevError = tError;
        rPrevError = rError;
        pros::delay(20);
    }
    setDrive(0, 0);
}

void leftAuto(){

}

void rightAuto(){

}

void skills(){
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
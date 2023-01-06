#include "main.h"
#include "pros/rtos.hpp"


bool activePID = true;
//silly tuners
double tkP=0.3, tkI=0, tkD=1, rkP=1, rkI=0, rkD=0;


void drivePID(int dist, int angle, int driveCAP) {
    //transform shennanigans
    double tError = 0;
    double tPrevError = 0;
    double tDerivative = 0;
    double tIntegral = 0;

    //goofy rotate
    double  rError = 0;
    double rPrevError = 0;
    double  rDerivative = 0;
    double rIntegral = 0;

    gyro.tare();
    resetEncoders();

    while(fabs(rError) > 2 && fabs(tError) > 1*12.5008973483){ //PID runs as long as angle is 2 deg away from desired and dist is 1/100th a tile away from desired 
        //transform
        tError = dist * 12.5008973483 - avgEncoderValue();
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
            setDrive((int) transform, (int) transform);
        } else {
        setDrive((int)transform + (int)rotate, (int)transform - (int) rotate);
        }

        //setting reload for cata
        if(limit.get_value() == 0) catapult = 127;

        //setting intake AND roller (mostly used for intake)
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
    reload();
    drivePID(-250,0,60);

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
    drivePID(-110,0,60);
    drivePID(110,0,127);
    drivePID(0,0,127); //dunno angle
    launch();
    drivePID(0,0,127); //dunno angle

//---------------pick up 1 disc and line up for other two--------------------------
    drivePID(0,0,127); //dunno angle
    reload();
    drivePID(-150,0,127); //dunno dist
    drivePID(-50,0,60); //dunno dist
    drivePID(25,0,127);
    drivePID(0,0,127); //dunno angle

//---------------------pick up other 2 discs & shoot by lowgoal corner---------------------------------
    drivePID(-161,0,60); //141 is abt sqrt(2);
    drivePID(20,0,127);
    drivePID(0,90,127);
    drivePID(0,0,127); //dunno dist
    launch();
    drivePID(-0,0,127); //dunno dist
    drivePID(0,-90,127);
  
  //-------------------------------getting 3 stack, shooting, & aligning w roller---------------------
    drivePID(-121,0,127); //this line and the following add up to abt sqrt(2)
    reload();
    drivePID(-161,0,50);
    drivePID(0,45,127);
    drivePID(250,0,127);
    launch();
    drivePID(-250,0,127); 
    drivePID(0,-90,127);
    reload();
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
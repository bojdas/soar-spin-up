#include "main.h"
#include "pros/rtos.hpp"
#include "subHeaders/drive.hpp"
#include "subHeaders/flywheel.hpp"
#include "subHeaders/globals.hpp"
#include "subHeaders/intake.hpp"

bool activePID = true;
//silly tuners
double tkP=0.5, tkI=0, tkD=5.5, rkP=2.5, rkI=0, rkD=10;

//transform shennanigans
int tError = 0;
int tPrevError = 0;
int tDerivative = 0;
int tIntegral = 0;

//goofy rotate
int rError = 0;
int rPrevError = 0; //20 ms intervals
int rDerivative = 0;
int rIntegral = 0;
//incase encoder reset needed
bool reset = true;

void drivePID(int dist, int angle, int driveCAP) {
    gyro.tare();
    while(gyro.get_rotation() != angle && avgEncoderValue() != dist){
        //transform
        tError = dist * 11.5 - avgEncoderValue();
        tDerivative = tError - tPrevError;
        tIntegral += tError;
        if ((tError = 0)  || (tError > dist))
            tIntegral = 0;
        double transform = tError * tkP + tDerivative * tkD + tIntegral * tkI;

        //rotate
        double turnDiff = gyro.get_rotation();
        rError =  angle - turnDiff;
        rDerivative = rError - rPrevError;
       // rIntegral += rError;
        //if ((rError = 0)  || (rError > angle))
           // rIntegral = 0;
        double rotate = rError * rkP + rDerivative * rkD;
        // rIntegral * rkI;

        if(transform > driveCAP) transform = driveCAP;
        if(transform < -driveCAP) transform = -driveCAP;
        //setting drive
        if(transform == 0) {
            setDrive(rotate, -rotate);
        } else if(rotate == 0) {
            setDrive(transform, -transform);
        } else {
        setDrive((int)transform + (int)rotate, (int)transform - (int) rotate);
        }
        //setting prev errors
        tPrevError = tError;
        rPrevError = rError;
    }
}

void leftAuto(){
    /*translate(-130, 50, 1000);
    pros::delay(100);
    setDriveTime(-15, -15, 400);
    autoIntake(610, 127);
    translate(170, 50, 1000);
    pros::delay(100);
    rotate(2.7, 50);
    pros::delay(100);
    translate(120, 50, 1000);
    setFlywheelandIndTime(480, -100);*/
}

void rightAuto(){
    translate(300, 60, 2000);
    rotate(73, 50);
    translate(-1100, 80,4000);
    rotate(-70, 50);
    translate(-400, 50, 1500);
    setDriveTime(-15, -15, 400);
    autoIntake(550, 127);
    translate(300, 50, 1000);
    pros::delay(100);
    rotate(-1.5, 30);
    pros::delay(100);
    // translate(120, 50, 1000);
    setFlywheelandIndTime(485, -110);
}

void skills(){
    translate(-130, 50, 1000);
    pros::delay(100);
    autoIntake(400, 127);
    translate(170, 50, 1000);
    pros::delay(100);
    rotate(2.7, 50);
    pros::delay(100);
    translate(120, 50, 1000);
    setFlywheelandIndTime(490, -100);
    translate(800, 40, 2000);
    rotate(-80, 50);
    translate(-1200, 50, 2000);
    pros::delay(300);
    autoIntake(370, 127);
    translate(800, 60, 1000);
    rotate(50, 40);

    endgame.set_value(true);
}

void skillsRoutingTestFULL(){
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
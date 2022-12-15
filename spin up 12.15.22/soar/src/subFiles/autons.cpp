#include "main.h"
#include "pros/rtos.hpp"
#include "subHeaders/drive.hpp"
#include "subHeaders/flywheel.hpp"
#include "subHeaders/globals.hpp"
#include "subHeaders/intake.hpp"

bool activePID = true;
//silly tuners
double tkP=0, tkI=0, tkD=0, rkP=0, rkI=0, rkD=0;

//target transform and rotate
int tTarget = 0;
int rTarget = 0;

//transform shennanigans
int tError;
int tPrevError = 0;
int tDerivative;
int tIntegral = 0;

//goofy rotate
int rError;
int rPrevError = 0; //20 ms intervals
int rDerivative;
int rIntegral = 0;

//incase encoder reset needed
bool reset = false;

int drivePID() {
    while(activePID){
        if(reset){
            resetEncoders();
            reset = false;
        }
        //transform
        double averagePos = avgEncoderValue();
        tError = averagePos - tTarget;
        tDerivative = tError - tPrevError;
        tIntegral += tError;
        if ((tError = 0)  || (tError > tTarget))
            tIntegral = 0;
        double transform = tError * tkP + tDerivative * tkD + tIntegral * tkI;

        //rotate
        double turnDiff =(lBack.get_position()+lMid.get_position()+lFront.get_position())/3 - (rBack.get_position()+rMid.get_position()+rFront.get_position())/3;
        rError = turnDiff - rTarget;
        rDerivative = rError - rPrevError;
        rIntegral += rError;
        if ((rError = 0)  || (rError > rTarget))
            rIntegral = 0;
        double rotate = rError * rkP + rDerivative * rkD + rIntegral * rkI;

        //setting drive motors to whatever the hell
        setDrive(transform + rotate, transform - rotate);
        tPrevError = tError;
        rPrevError = rError;
        pros::Task::delay(20);
    }
    return 1;
}
    /*pros::Task task(drivePID);
    reset = true;
    //wtvr steps
    tTarget = 300; //hopefully 300 forward (preferably cm)
    rTarget = 90; //hopefully turns 90 degrees
    pros::Task::delay(1000); //sleep a sec*/
void leftAuto(){
    translate(-130, 50, 1000);
    pros::delay(100);
    setDriveTime(-15, -15, 400);
    autoIntake(610, 127);
    translate(170, 50, 1000);
    pros::delay(100);
    rotate(2.7, 50);
    pros::delay(100);
    translate(120, 50, 1000);
    setFlywheelandIndTime(480, -100);
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
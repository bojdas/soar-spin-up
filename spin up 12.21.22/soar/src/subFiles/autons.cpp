#include "main.h"
#include "pros/rtos.hpp"
#include "subHeaders/drive.hpp"
#include "subHeaders/flywheel.hpp"
#include "subHeaders/globals.hpp"
#include "subHeaders/intake.hpp"

bool activePID = true;
//silly tuners
double tkP=0.5, tkI=0, tkD=5.5, rkP=0, rkI=0, rkD=0;

//target transform and rotate
int tTarget = 100;
int rTarget = 0;

//transform shennanigans
int tError = 0;
int tPrevError = 0;
int tDerivative;
int tIntegral = 0;

//goofy rotate
int rError = 0;
int rPrevError = 0; //20 ms intervals
int rDerivative;
int rIntegral = 0;

//incase encoder reset needed
bool reset = true;

int drivePID() {
    while(activePID){
        if(reset){
            resetEncoders();
            reset = false;
        }
        //transform
        tError = tTarget*12 - avgEncoderValue();
        tDerivative = tError - tPrevError;
        /*tIntegral += tError;
        if ((tError = 0)  || (tError > tTarget))
            tIntegral = 0;*/
        double transform = tError * tkP + tDerivative * tkD;
        //tIntegral * tkI;

        //rotate
        double turnDiff =(lBack.get_position()+lMid.get_position()+lFront.get_position())/3 - (rBack.get_position()+rMid.get_position()+rFront.get_position())/3;
        rError = turnDiff - rTarget;
        rDerivative = rError - rPrevError;
       // rIntegral += rError;
        //if ((rError = 0)  || (rError > rTarget))
           // rIntegral = 0;
        double rotate = rError * rkP + rDerivative * rkD;
        // rIntegral * rkI;

        //setting drive
        setDrive((int)transform, (int)transform);
        //setting prev errors
        tPrevError = tError;
       // rPrevError = rError;

        //cuz task
        pros::Task::delay(10);
    }
    return 1;
}

void leftAuto(){
    pros::Task task(drivePID);
    pros::delay(1500);
    tTarget+=100;
    pros::delay(2500);
    tTarget-=200;
    pros::delay(3000);
    tTarget=200;
    pros::delay(3000);
    rTarget=90;
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
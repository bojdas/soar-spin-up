#include "main.h"
#include "ARMS/config.h"
#include <cmath>
#include <iostream>

pros::Controller master(CONTROLLER_MASTER);

pros::Motor catapult(20, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor intake(14, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);

pros::ADIDigitalIn limit ('A');
pros::ADIDigitalOut endgame('C');
pros::ADIDigitalOut boosters('B');

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
    master.rumble("-");
   catapult = 0;
}
void launch() {
    master.rumble(".");
    while((limit.get_value() == 1)) {
   catapult = 127;
    }
    master.rumble(".");
   catapult = 0;
}

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
    using namespace arms::chassis;
    move(16.8, 100);

    turn(90, 100, arms::RELATIVE);

    move(-3.6, 100, arms::REVERSE);

    rollerPID(-0.15, 1000);

    move(3.6, 100);

    turn(95,100, arms::RELATIVE);

    launch();

    turn(135,100, arms::RELATIVE);

    reload();

    intake = 127;

    move(-57.84,50,arms::REVERSE);

    turn(90, 100, arms::RELATIVE);

    move(4.8,100);

    pros::delay(1500);

    intake = 0;

    reload();

    pros::delay(3000);

    launch();


    std::cout << "end of right auton reached";
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

    intake = 127;

    move(-25.92,60);

    turn(-96,100, arms::RELATIVE);

    move(9.6,100);

    pros::delay(1000);

    intake = 0;

    pros::delay(3000);

    launch();

    reload();

    std::cout << "end of left auton reached";

}

void newSkills() {
    using namespace arms::chassis;
    move(-3.6, 100, arms::REVERSE);
    rollerPID(-0.2, 1000);
    move(3.6,100);
    turn(-120,100);
    pros::delay(100);
    intake = 127;
    move(-80,50,arms::REVERSE);
    pros::delay(750);
    intake = 0;
    turn(43,100, arms::RELATIVE);
    pros::delay(100);
    move(-35,100, arms::REVERSE);
    rollerPID(-0.4, 1000);
    move(15,100);
    turn(90,100, arms::RELATIVE);
    pros::delay(100);
    move(215,100);
    turn(-5,100, arms::RELATIVE);
    pros::delay(2000);
    //launch();
    turn(100,100, arms::RELATIVE);
    reload();
    intake = 127;
    move(-110,100,arms::REVERSE);
    move(120,100,arms::REVERSE);
    turn(-100,100, arms::RELATIVE);
    intake = 0;
    pros::delay(2000);
    //launch();
    turn(50,100, arms::RELATIVE);
    intake = 127;
    move(-132, 100, arms::REVERSE);
    turn(100,100, arms::RELATIVE);
    move(-136, 100, arms::REVERSE);
    move(15,100);
    intake = 0;
    turn(-90,100,arms::RELATIVE);
    move(50,100);
    pros::delay(2000);
    //launch();
    move(-50,100,arms::REVERSE);
    turn(90,100, arms::RELATIVE);
    move(-121,100,arms::REVERSE);
    reload();
    intake = 127;
    move(-50,50,arms::REVERSE);
    intake = 0;
    move(-95,70,arms::REVERSE);
    rollerPID(-0.4, 1000);
    move(15,100);
    turn(-45,100, arms::RELATIVE);
    pros::delay(100);
    move(200,100);
    turn(5,100, arms::RELATIVE);
    pros::delay(2000);
    //launch();

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
#pragma once
#include "main.h"


// motors
extern pros::Motor rMid;
extern pros::Motor intake;
extern pros::Motor lMid;
extern pros::Motor catapult;
extern pros::Motor lBack;
extern pros::Motor lFront;
extern pros::Motor rBack;
extern pros::Motor rFront;

//motor groups whee
extern pros::Motor_Group leftDrive;
extern pros::Motor_Group rightDrive;

// controller
extern pros::Controller cece;

// misc
extern pros::ADIDigitalIn limit;
//extern pros::ADIDigitalOut endgame;
extern pros::Imu gyro;

//pistons
extern pros::ADIDigitalOut endgame;
extern pros::ADIDigitalOut boosterL;
extern pros::ADIDigitalOut boosterR;

//nonConstructor
extern int autonIntakePower;
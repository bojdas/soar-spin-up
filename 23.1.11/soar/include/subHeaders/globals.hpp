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
extern pros::ADIDigitalOut endgame;
extern pros::ADIDigitalOut boosters;
extern pros::Imu gyro;

//nonConstructor
extern int autonIntakePower;
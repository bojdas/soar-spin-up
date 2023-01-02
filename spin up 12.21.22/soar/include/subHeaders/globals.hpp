#pragma once
#include "main.h"
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/motors.hpp"

// motors
extern pros::Motor rMid;
extern pros::Motor intake;
extern pros::Motor lMid;
extern pros::Motor flywheel;
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
extern pros::ADIDigitalOut endgame;

extern pros::Imu gyro;
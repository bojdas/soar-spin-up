#pragma once
#include "main.h"

//helpers
void resetEncoders();
double avgEncoderValue();
void setDrive(int left, int right);

//joystick mapping
double trial(int i, bool side);
void Mapping(int i, bool side);

//slew rate
int slewRate();

//controller mapping
pros::controller_digital_e_t controllerMapping(const char *function, pros::controller_digital_e_t usual);
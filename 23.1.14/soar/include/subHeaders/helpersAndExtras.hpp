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

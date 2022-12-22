#pragma once
#include "main.h"

// helper funcs
void setDrive(int left, int right);
void setDriveTime(int left, int right, int time);
void resetEncoders();
double avgEncoderValue();
int drivePID();

// driver control funcs
void setDriveMotors();

// auton funcs
void translate(int units, int volts, int timeout);
void rotate(float degrees, int volts);
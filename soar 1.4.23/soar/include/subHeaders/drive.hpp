#pragma once
#include "main.h"

// helper funcs
void setDrive(int left, int right);
void setDriveTime(int left, int right, int time);
void resetEncoders();
double avgEncoderValue();

// driver control funcs
void setDriveMotors();

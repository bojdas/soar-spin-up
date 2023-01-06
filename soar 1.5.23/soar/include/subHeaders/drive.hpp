#pragma once
#include "main.h"

// helper funcs
void setDrive(int left, int right);
void resetEncoders();
double avgEncoderValue();

// driver control funcs
void setDriveMotors();

#pragma once
#include "main.h"

//helpers
void resetEncoders();
double avgEncoderValue();

//cata
void setCataMotor(bool autoReload);

//drive
void setDrive(int left, int right);
void setDriveMotors();

//intake
void setIntakeMotors();
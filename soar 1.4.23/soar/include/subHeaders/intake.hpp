#pragma once
#include "main.h"

// helpers
void setIntake(int intPwr);

// driver control
void setIntakeMotors();
void intakeToggle();
void setIntakeTime(int intPwr, int time);

void autoIntake(int dist, int volts);
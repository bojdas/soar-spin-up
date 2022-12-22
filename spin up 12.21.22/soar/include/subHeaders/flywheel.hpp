#pragma once
#include "main.h"
#include "pros/rtos.hpp"

void takeBackHalf(int target);
void setFlywheel(int fPwr);
void pLoop(int target);

// driver control funcs
void setFlywheelMotors();
void setFlywheelandIndTime(int pwr, int indPwr);
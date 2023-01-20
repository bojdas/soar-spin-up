#pragma once
#include "main.h"

//auton
void distPID(int dist, int driveCAP);
void turnPID(int angle, int driveCAP);
void drivePID(int dist, int angle, int driveCAP);
void reload();
void launch();
//paths
void leftAuto();
void rightAuto();
void skills();
void skillsRoutingTest();
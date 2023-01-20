#pragma once
#include "main.h"

//auton
void drivePID(int dist, int angle, int driveCAP);
void turnPID(int angle, int driveCAP, int timeout);
void distPID(int angle, int driveCAP, int timeout);
void reload();
void launch();
//paths
void leftAuto();
void rightAuto();
void newSkills();
void skills();
void skillsRoutingTest();
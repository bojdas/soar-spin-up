#pragma once
#include "main.h"

//auton
void turnPID(int angle, int driveCAP, int timeout, double kP, double kD, bool relative, int settleTime);
void distPID(int dist, int driveCAP, int timeout, double kP, double kD);
void rollerPID(double rotation, int timeout);

void reload();
void launch();
//paths
void leftAuto();
void rightAuto();
void newSkills();
//void skills();
//void skillsRoutingTest();
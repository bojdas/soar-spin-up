#pragma once
#include "main.h"

//auton
void rollerPID(double rotation, int timeout);

void distPID(int dist, int driveCAP, int timeout, double kP, double kD, int settleTime);
void distPID(int dist, int driveCAP, int timeout, double kP, double kD);

void turnPID(int angle, int driveCAP, int timeout, double kP, double kD, bool relative, int settleTime);
void turnPID(int angle, int driveCAP, int timeout);
void turnPID(int angle, int timeout);
void turnPID(int angle, int driveCAP, int timeout, bool relative, int settleTime);

void reload();
void launch();
//paths
void leftAuto();
void rightAuto();
void skils();
//void skills();
//void skillsRoutingTest();
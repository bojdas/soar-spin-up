#pragma once
#include "main.h"

//auton
void drivePID(int dist, int angle, int driveCAP, int timeout);
void reload();
void launch();
//paths
void leftAuto();
void rightAuto();
void skills();
void skillsRoutingTest();

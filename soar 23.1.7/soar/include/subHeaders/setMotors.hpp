#pragma once
#include "main.h"

//cata
void setCataMotor(bool autoReload, const char *launch, const char *reload);
//drive
void setDriveMotors();

//intake
void setIntakeMotors(const char *intaking, const char *outtaking);
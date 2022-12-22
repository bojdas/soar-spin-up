#pragma once

#include <string>

//selector configuration
#define HUE 360
#define DEFAULT 8
#define AUTONS "One", "Two", "Three", "Do Nothing"

namespace selector{

extern int auton;
//const char *autonomousChoices[] = {AUTONS, ""};
void init(int hue = HUE, int default_auton = DEFAULT);

}

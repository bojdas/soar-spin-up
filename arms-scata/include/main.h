#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

#define PROS_USE_SIMPLE_NAMES
#define PROS_USE_LITERALS

#include "ARMS/api.h"
#include "api.h"

using namespace pros;

extern ADIDigitalIn limit;
extern ADIDigitalOut boosters;
extern Imu gyro;
extern Controller master;

#ifdef __cplusplus
extern "C" {
#endif
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/**
 * You can add C++-only headers here
 */
//#include <iostream> 
#endif

#endif  // _PROS_MAIN_H_

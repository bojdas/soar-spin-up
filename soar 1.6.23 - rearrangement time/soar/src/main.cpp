#include "main.h"
#include "subHeaders/autons.hpp"
#include "subHeaders/globals.hpp"
#include "subHeaders/joystickMapping.hpp"
#include <string>

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    lBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    lFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    rBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    rFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    catapult.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    lMid.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    rMid.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    gyro.reset();

    pros::delay(2000);

    selector::init();

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {

}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
    int init = pros::millis();
    //drivePID(200, 0, 127);
    //drivePID(0, 90, 60);

    // leftAuto();
    //rightAuto();
    // skills();
    switch(selector::auton) {
        // left Autons
        case -1: {
            Mapping(0, true);
            break;
            }
        case -2: {
            Mapping(1, true);
            break;
            }
        case -3: {
            Mapping(2, true);
            break;
            }
        case -4: {
            Mapping(3, true);
            break;
            }

        // right Autons
        case 1: {
            Mapping(4, true);
            break;
            }
        case 2: {
            Mapping(5, true);
            break;
            }
        case 3: {
            Mapping(6, true);
            break;
            }
        case 4: {
            Mapping(7, true);
            break;
            }

        //skills
        case 5:  {
            Mapping(8, true);
            break;
            }
        case 6: {
            Mapping(9, true);
            break;
            }
        case 7: {
            Mapping(10, true);
            break;
            }
        case 8: {
            Mapping(11, true);
            break;
            }
        default:
            break;
}
    init = pros::millis() - init;
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
    int startTime = pros::millis();
    bool autoReload = true;
	while(true){
        if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
         //   endgame.set_value(true);
         cece.rumble(".");
        }
        if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)){
         //   boosters.set_value(false);
         cece.rumble(".");
        }
        if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_A)  && (pros::millis()-startTime)%200==0){
            if(autoReload) {
                autoReload = false;
                cece.rumble("...");
            } else {
                autoReload = true;
                cece.rumble("..");
            }
        }

      cece.print(0, 0, "limiter: %d", limit.get_value());
      cece.print(1, 0, "autoReload: %d", autoReload);

        //td: drive code
        setDriveMotors();
        //td: intake coderos 
        setIntakeMotors();
        //td: flywheel code
        setCataMotor(autoReload);
        pros::delay(10);
    }
}

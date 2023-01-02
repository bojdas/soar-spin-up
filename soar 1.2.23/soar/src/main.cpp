#include "main.h"
#include "subHeaders/autons.hpp"
#include "subHeaders/drive.hpp"
#include "subHeaders/globals.hpp"
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
    flywheel.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
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
     leftAuto();
    //rightAuto();
    // skills();
    switch(selector::auton) {
        // left Autons
        case -1:
            break;
        case -2:
            break;
        case -3:
            break;
        case -4:
            break;

        // right Autons
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;

        //skills
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8: {
            skillsRoutingTest();
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
	while(true){
        if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
            endgame.set_value(true);
        }

        if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)){
            endgame.set_value(false);
        }
        //td: drive code
        setDriveMotors();
        //td: intake coderos 
        setIntakeMotors();
        //td: flywheel code
        setFlywheelMotors();
        pros::delay(10);
    }
}

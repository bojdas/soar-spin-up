#include "main.h"
#include "ARMS/chassis.h"
#include "autons.hpp"
#include "ARMS/config.h"
#include "pros/misc.h"
#include "pros/rtos.hpp"

pros::Controller master(CONTROLLER_MASTER);

//motor thingy
pros::Motor motor1(15, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor motor2(17, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor_Group motorGroup ({motor1, motor2});

//lefts
pros::Motor lFront(1, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lMid(2, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lBack(3, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);

//rights
pros::Motor rFront(10, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rMid(9, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rBack(8, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor_Group leftDrive ({lFront, lMid, lBack});
pros::Motor_Group rightDrive ({rFront, rMid, rBack});

pros::Imu gyro(16); //CHANGEA LTER
pros::ADIDigitalIn limit ('A');
pros::ADIDigitalOut endgame('C');
pros::ADIDigitalOut boosters('B');


void initialize() {
	arms::init();
}

void disabled() {
}

void competition_initialize() {
}

void autonomous() {
    double init = pros::millis();
	//skills();
    //leftAuto();
    //rightAuto();
	using namespace arms::chassis;
	move(24,10);

    switch(arms::selector::auton) {
        // left Autons
        case -1: {
            //leftAuto();
            break;
            }
        case -2: {
           // leftAuto();
            break;
            }
        case -3: {
            break;
            }
        case -4: {
            break;
            }

        // right Autons
        case 1: {
           // rightAuto();
            break;
            }
        case 2: {
           // rightAuto();
            break;
            }
        case 3: {
            break;
            }
        case 4: {
            break;
            }

        //skills
        case 5:  {
           // newSkills();
            break;
            }
        case 6: {
            break;
            }
        case 7: {
            break;
            }
        case 8: {
            break;
            }
        default:
            break;
	}
    std::cout << "auton total time: " << pros::millis() - init << "\n";
}

void opcontrol() {
    bool autoReload = false;
    bool boostersState = false;
    bool expansionState = false;
    double init = pros::millis();
	while (true) {
        int power = master.get_analog(ANALOG_LEFT_Y); // power of forward/backward movement
        int turn = master.get_analog(ANALOG_RIGHT_X); // power of turn movement

        int left = power + 0.85*turn;
        int right = power - 0.85*turn;

        if(left > 5) {
            left = 0.2*left+0.0000427401574*pow(left,3)+15;
         }  else if (left < -5) {
            left = 0.2*left+0.0000427401574*pow(left,3)-15;
         } else {
             left = 0;
         }

        if(right > 5) {
            right = 0.2*right+0.0000427401574*pow(right,3)+15;
         }  else if (left < -5) {
            right = 0.2*right+0.0000427401574*pow(right,3)-15;
         } else {
             right = 0;
         }

        leftDrive = left;
        rightDrive = right;

		//arms::chassis::arcade(master.get_analog(ANALOG_LEFT_Y) * (double)100 / 127,master.get_analog(ANALOG_RIGHT_X) * (double)100 /127);
	//----------------------------ENDGAME-------------------------------
		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
			if(expansionState) {
				endgame.set_value(false);
				expansionState = false;
				master.rumble("....");
			} else {
				endgame.set_value(true);
				expansionState = true;
				master.rumble("...");
			}
		}

	//----------------------------AUTORELOAD-------------------------------
		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){
			if(autoReload) {
				autoReload = false;
				master.rumble("-.");
			} else {
				autoReload = true;
				master.rumble("-..");
			}
		}

	/*
	//----------------------------BAND RELEASE-------------------------------
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
			if(boostersState) {
				boosters.set_value(false);
				boostersState = false;
				master.rumble(".-");
			} else {
				boosters.set_value(true);
				boostersState = true;
				master.rumble(".--");
			}
		}

		if(limit.get_new_press() == 1) {
			master.rumble("-");
		} */

	//----------------------------CATAPULT & INTAKE-------------------------------
	int motors = 0;
	//launch
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && (limit.get_value() == 1)) {
			motors = 127;
		}
		//manual reload
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) && (limit.get_value() == 0)) {
			motors = 127;
		}
		//auto reload
		if(autoReload && (limit.get_value() == 0)) {
			motors = 127;
		}
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) && limit.get_value() == 1){
			motors = -127;
		}

		motorGroup = motors;

        //debugging piston boosters
        if(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
            boosters.set_value(true);
            pros::delay(100);
            while(limit.get_value() == 1) {
                motorGroup = 127;
                pros::delay(10);
            }
            pros::delay(100);
            boosters.set_value(false);
        }

		//--------------DELAY AND PRITNS--------------
		//master.print(0, 0, "limit: %d", limit.get_value());
		master.print(0, 0, "TIME: %d", 105-(pros::millis()-init/1000));

		pros::delay(10);
	}
}

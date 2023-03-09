#include "main.h"
#include "ARMS/chassis.h"
#include "autons.hpp"
#include "ARMS/config.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"

pros::Controller master(CONTROLLER_MASTER);

//scata + intake + roller
pros::Motor leftM(1, pros::E_MOTOR_GEAR_200, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rightM(9, pros::E_MOTOR_GEAR_200, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor_Group motorGroup ({leftM, rightM});

//lefts
pros::Motor lTop(13, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lBottom(12, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lFront(11, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor_Group leftDrive ({lTop, lBottom, lFront});

//rights
pros::Motor rTop(14, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rBottom(16, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rFront(18, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor_Group rightDrive ({rTop, rBottom, rFront});


pros::Imu gyro(16); //CHANGEA LTER
pros::ADIDigitalIn limit ('H');
pros::ADIDigitalOut endgame('A');
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

    std::cout << "auton total time: " << pros::millis() - init << "\n";
}

void  debug() {
    while(1) {
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
         }  else if (right < -5) {
          right = 0.2*right+0.0000427401574*pow(right,3)-15;
         } else {
             right = 0;
         }

        leftDrive = left;
        rightDrive = right;

        if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
            boosters.set_value(true);
            pros::delay(100);
            while(limit.get_value() == 1) {
                motorGroup = 127;
                pros::delay(10);
            }
            pros::delay(100);
            boosters.set_value(false);
        }
        //----------------------------CATAPULT & INTAKE-------------------------------
        if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            motorGroup = 127;
        } else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            motorGroup = -127;
        } else {
            motorGroup = 0;
        }

        pros::delay(10);
    }
}

void opcontrol() {
    bool autoReload = false;
    bool boostersState = false;
    bool expansionState = false;
    long init = pros::millis();
    //debug();

	while (true) {
	//---------------------------------DRIVE------------------------------------
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
         }  else if (right < -5) {
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
        //intake
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) && limit.get_value() == 1){
			motors = -127;
		}

        motorGroup = motors;


		//--------------DELAY AND PRITNS--------------
		master.print(0, 0, "limit: %d", limit.get_value());
        if(((double)105-((int)pros::millis()/(double)1000-init/(double)1000)) == 45)
			master.rumble(".");
        if(((double)105-((int)pros::millis()/(double)1000-init/(double)1000)) == 30)
			master.rumble("..");
        if(((double)105-((int)pros::millis()/(double)1000-init/(double)1000)) == 15)
			master.rumble("...");
		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2))
			std::cout << "time: " << (double)105-((int)pros::millis()/(double)1000-init/(double)1000) << "\n";
		pros::delay(10);

	}
}
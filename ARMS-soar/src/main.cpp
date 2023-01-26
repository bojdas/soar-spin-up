#include "main.h"
#include "ARMS/config.h"

pros::Controller master(CONTROLLER_MASTER);

pros::Motor catapult(20, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor intake(14, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);

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
	using namespace arms;
    int init = pros::millis();
    switch(selector::auton) {
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
    init = pros::millis() - init;
    std::cout << "end of auton FUNC reached @time: " << init;
}

void opcontrol() {
    bool autoReload = true;
    bool boostersState = false;
    bool expansionState = false;

	while (true) {
		//----------------------------DRIVE-------------------------------
		arms::chassis::arcade(master.get_analog(ANALOG_LEFT_Y) * (double)100 / 127, master.get_analog(ANALOG_RIGHT_X) * (double)100 /127);
        
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
        }

		//----------------------------INTAKE-------------------------------
		int intakePower = 0;
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) && limit.get_value() == 1){
			intakePower = 127;
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
			intakePower = -127;
		}
		intake = intakePower;

		//----------------------------CATAPULT-------------------------------
		int cataPower = 0;
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) && (limit.get_value() == 0)) {
			cataPower = 127;
		}
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && (limit.get_value() == 1)) {
			cataPower = 70;
		}
		if(autoReload && (limit.get_value() == 0)) {
			cataPower = 127;
		}
	catapult = cataPower; 


		pros::delay(10);
	}
}

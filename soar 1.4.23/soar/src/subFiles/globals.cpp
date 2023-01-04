#include "main.h" 

// define motors
pros::Motor catapult(20, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor intake(11, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);

//lefts
pros::Motor lFront(19, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lMid(18, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lBack(9, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);

//rights
pros::Motor rFront(12, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rMid(13, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rBack(3, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor_Group leftDrive ({lBack, lMid, lFront});
pros::Motor_Group rightDrive ({rBack, rMid, rFront});

// define controller
pros::Controller cece(pros::E_CONTROLLER_MASTER);

// define misc
pros::ADIDigitalIn limit (1);

//pros::ADIDigitalOut endgame(');

pros::Imu gyro(8);

int autonIntakePower = 0;
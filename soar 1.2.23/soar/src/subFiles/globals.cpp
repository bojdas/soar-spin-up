#include "main.h" 

// define motors
pros::Motor flywheel(18, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor intake(5, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rMid(9, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lMid(21, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lBack(8, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lFront(10, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rBack(8, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rFront(10, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor_Group leftDrive ({lBack, lMid, lFront});
pros::Motor_Group rightDrive ({rBack, rMid, rFront});

// define controller
pros::Controller cece(pros::E_CONTROLLER_MASTER);

// define misc
pros::ADIDigitalOut endgame('A');

pros::Imu gyro(8);

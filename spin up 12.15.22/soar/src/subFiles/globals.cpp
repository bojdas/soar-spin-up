#include "main.h" 

// define motors
pros::Motor flywheel(12, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor intake(20, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rMid(10, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lMid(4, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lBack(5, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lFront(2, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rBack(15, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rFront(21, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);

// define controller
pros::Controller cece(pros::E_CONTROLLER_MASTER);

// define misc
pros::ADIDigitalOut endgame('A');

pros::Imu gyro(8);

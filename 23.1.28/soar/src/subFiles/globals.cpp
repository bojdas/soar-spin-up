#include "main.h" 

// define motors
pros::Motor catapult(20, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor intake(14, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);

//lefts
pros::Motor lFront(19, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lMid(18, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lBack(9, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);

//rights
pros::Motor rFront(12, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rMid(13, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rBack(3, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor_Group leftDrive ({lFront, lMid, lBack});
pros::Motor_Group rightDrive ({rFront, rMid, rBack});

// define controller
pros::Controller cece(pros::E_CONTROLLER_MASTER);

// define misc
pros::ADIDigitalIn limit ('A');

pros::ADIDigitalOut endgame('C');
pros::ADIDigitalOut boosters('B');

pros::Imu gyro(8);
#include "main.h"

// driver control funcs
void setCataMotor(){
    int power = 0;
    // bottom cata down, top cata up
    if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_R2) && !(limit.get_value())){
        catapult = 127;
    }
    if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_R1) {
        launch();
    })
}

void reload() {
    while(!limiter.get_value()) {
        catapult = 127;
    }
}
void launch() {
    int dist = 100, int velocity = 100;
  catapult.move_relative(dist, velocity);
  while (!((catapult.get_position() < catapult.get_position()+dist + 5) && (motor.get_position() > moto.get_position()+dist-5))) {
    pros::delay(10);
  }
  cece.rumble(".");
}
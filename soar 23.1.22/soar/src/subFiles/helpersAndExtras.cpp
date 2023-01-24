#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_label.h"
#include "main.h" 
#include "pros/rtos.hpp"
#include "subHeaders/globals.hpp"
#include "subHeaders/setMotors.hpp"

int joystickMap[128] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
//helpers
void setDrive(int left, int right){
    if(left > 127) left = 127;
    if(right > 127) right = 127;
    if(left < -127) left = -127;
    if(right < -127) right = -127;

    //left = joystickMap[left];
    //right = joystickMap[right];

    leftDrive = left;
    rightDrive = right;
}

void resetEncoders(){
    lBack.tare_position();
    lFront.tare_position();
    lMid.tare_position();
    rMid.tare_position();
    rBack.tare_position();
    rFront.tare_position();
}

double avgEncoderValue(){
    return (lBack.get_position()
            + lFront.get_position()
            + lMid.get_position()
            + rMid.get_position()
            + rBack.get_position()
            + rFront.get_position()) / 6;
}

double trial(int i, bool side) {
    double rpm = 0;
    if(side) {
        leftDrive = i;
        pros::delay(2000);
        resetEncoders();
        pros::delay(5000);
        rpm = (((lFront.get_position() + lMid.get_position() + lBack.get_position())/3.0)/(900.0*0.5))*12.0;
        leftDrive = 0;
    } else {
       rightDrive = i;
        pros::delay(2000);
        resetEncoders();
        pros::delay(5000);
        rpm = (((rFront.get_position() + rMid.get_position() + rBack.get_position())/3.0)/(900.0*0.5))*12.0;
        rightDrive = 0;
    }
    pros::delay(3000);
      cece.print(0, 0, "rpm: %f", rpm);
    std::cout << "trial: " << rpm << "\n";
    return rpm;
}

void Mapping(int i, bool side) {
    double t1 = 0, t2 = 0, t3 = 0;
    t1 = trial(i, side);
    while (!cece.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
        pros::delay(100);
    }
    t2 = trial(i, side);
    while (!cece.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
        pros::delay(100);
    }
    t3 = trial(i, side);
    double avg = (t1 + t2 +t3)/3.0;
    std::cout << "trial avg: " << avg << "\n";
}

/*
int slewRate() {
  int lError = 0, rError = 0, kS = 0.25, placedLeft = 0, placedRight = 0;
  while(true) {
    lError = left - (lBack + lMid + lFront)/3;
    rError = right - (rBack + rMid + rFront)/3;
    placedLeft += lError * kS;
    placedRight += rError *kS;

    rightDrive = placedRight;
    leftDrive = placedLeft;
    pros::delay(25);
  }
}
*/

lv_obj_t *debugOne;
lv_obj_t *debugTwo;
lv_obj_t *debugThree;

void setPrints(float vall, float val2, float val3) {
    lv_obj_clean(lv_scr_act());

    //value 1
    debugOne = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_long_mode (debugTwo, LV_LABEL_LONG_SCROLL);
    lv_obj_align(debugOne, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 0, 10);
    lv_label_set_recolor (debugOne, true);
    lv_obj_set_size (debugOne, 450, 35);
    
    //value 2
    lv_obj_t * debugTwo = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_long_mode (debugTwo, LV_LABEL_LONG_SCROLL);
    lv_obj_align(debugTwo, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 0, 45);
    lv_label_set_recolor (debugTwo, true);
    lv_obj_set_size (debugTwo, 450, 35);


    //value 3
    debugOne = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_long_mode (debugTwo, LV_LABEL_LONG_SCROLL);
    lv_obj_align(debugOne, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 0, 80);
    lv_label_set_recolor (debugOne, true);
    lv_obj_set_size (debugOne, 450, 35);
}

char *arr1;
char *arr2;
char *arr3;

void updatePrints(float val1, float val2, float val3) {

    //lv_label_set_text(debugOne, "rError: %f");
    // lv_label_set_text()
   // lv label set text_fmt (debugTwo, "rotate: %d", val2);
   // lv label set text fmt (debugThree, "gyro. get_rotation() : %d", val3);
   //lv_label_set_array_text(debugOne, arr1, val1);
}
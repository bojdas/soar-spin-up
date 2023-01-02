#include "main.h"

void launch(){
    if(cece.get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
        endgame.set_value(true);
    }
}
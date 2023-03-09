#include "api.h"

namespace arms::selector{

int auton;
int autonCount;
const char *btnmMap[] = {"One","Two","SAWP","Do Nothing","","","","","","",""}; // up to 10 autons MANUALLY SETTING AUTONCHOICES BC IDK

lv_obj_t *tabview;
lv_obj_t *leftBtnm;
lv_obj_t *rightBtnm;
lv_obj_t *skillsBtnm;

//action functions: set auton based off of butonmatrix selection and adjusts based off of tab open
lv_res_t leftBtnmAction(lv_obj_t *btnm, const char *txt){
	//printf("left button: %s released\n", txt);

	for(int i = 0; i < autonCount; i++){
		if(strcmp(txt, btnmMap[i]) == 0){
			auton = i+1;//1 to 3
		}
	}
	return LV_RES_OK; // return OK because the button matrix is not deleted
}

lv_res_t rightBtnmAction(lv_obj_t *btnm, const char *txt){
	//printf("right button: %s released\n", txt);

	for(int i = 0; i < autonCount; i++){
		if(strcmp(txt, btnmMap[i]) == 0){
			auton = -(i+1);//-1 to -3
		}
	}
	return LV_RES_OK; // return OK because the button matrix is not deleted
}

lv_res_t skillsBtnmAction(lv_obj_t *btn, const char *txt){
   //printf("skills pressed");

	for(int i = 0; i < autonCount; i++){ //NOTE: AUTONCOUNT MIGHT NOT WORK AND MAY NEED TO BE MADE MANUAL CONSTANT
		if(strcmp(txt, btnmMap[i]) == 0){
			auton = i+1+autonCount;//4 to 6
		}
	}	return LV_RES_OK;
}

//constantly looped detecting tab change
int tabWatcher() {
	int activeTab = lv_tabview_get_tab_act(tabview);
	while(1){
		int currentTab = lv_tabview_get_tab_act(tabview);

		if(currentTab != activeTab){
			activeTab = currentTab;
			if(activeTab == 0){ //if on the left
				if(auton == 0) auton = 1;
				if(auton < 0) auton = abs(auton);
				if(auton >autonCount) auton = abs(auton)-autonCount;
				lv_btnm_set_toggle(leftBtnm, true, abs(auton)-1);
			}else if(activeTab == 1){ //if on the right
				if(auton == 0) auton = -1;
				if(auton > 0 && auton < autonCount+1) auton = -abs(auton);
				if(auton >autonCount) auton = (-abs(auton))+autonCount;
				lv_btnm_set_toggle(rightBtnm, true, abs(auton)-1);
			}else if(activeTab == 2){ //if on skills
				if(auton == 0) auton = autonCount+1;
				if(auton < 0) auton = abs(auton)+autonCount;
				if(auton > 0 && auton < autonCount+1) auton = auton+autonCount;
				lv_btnm_set_toggle(skillsBtnm, true, abs(auton)-1-autonCount);
				}else {
				auton = 0;
			}
		}

		pros::delay(20);
	}
}


//INITIALIZATION
void init(int hue, int default_auton){

	autonCount = 4;
	auton = default_auton;
	

	// lvgl theme
	lv_theme_t *th = lv_theme_alien_init(hue, NULL); //Set a HUE value and keep font default RED
	lv_theme_set_current(th);

	// create a tab view object
	tabview = lv_tabview_create(lv_scr_act(), NULL);

	// add 3 tabs (the tabs are page (lv_page) and can be scrolled
	lv_obj_t *leftTab = lv_tabview_add_tab(tabview, "Left");
	lv_obj_t *rightTab = lv_tabview_add_tab(tabview, "Right");
	lv_obj_t *skillsTab = lv_tabview_add_tab(tabview, "Skills");

	//set default tab
	if(auton < 0){
		lv_tabview_set_tab_act(tabview, 1, LV_ANIM_NONE);
	}else if(auton >autonCount){
		lv_tabview_set_tab_act(tabview, 2, LV_ANIM_NONE);
	}

	// add content to the tabs
	// button matrix

	// left tab
	leftBtnm = lv_btnm_create(leftTab, NULL);
	lv_btnm_set_map(leftBtnm, btnmMap);
	lv_btnm_set_action(leftBtnm, *leftBtnmAction);
	lv_btnm_set_toggle(leftBtnm, true, abs(auton)-1);//3
	lv_obj_set_size(leftBtnm, 450, 50);
	lv_obj_set_pos(leftBtnm, 0, 100);
	lv_obj_align(leftBtnm, NULL, LV_ALIGN_CENTER, 0, 0);

	// right tab
	rightBtnm = lv_btnm_create(rightTab, NULL);
	lv_btnm_set_map(rightBtnm, btnmMap);
	lv_btnm_set_action(rightBtnm, *rightBtnmAction);
	lv_btnm_set_toggle(rightBtnm, true, abs(auton)-1);
	lv_obj_set_size(rightBtnm, 450, 50);
	lv_obj_set_pos(rightBtnm, 0, 100);
	lv_obj_align(rightBtnm, NULL, LV_ALIGN_CENTER, 0, 0);

	// skills tab
	skillsBtnm = lv_btnm_create(skillsTab, NULL);
	lv_btnm_set_map(skillsBtnm, btnmMap);
	lv_btnm_set_action(skillsBtnm, *skillsBtnmAction);
	lv_btnm_set_toggle(skillsBtnm, true, abs(auton)-1-autonCount);
	lv_obj_set_size(skillsBtnm, 450, 50);
	lv_obj_set_pos(skillsBtnm, 0, 100);
	lv_obj_align(skillsBtnm, NULL, LV_ALIGN_CENTER, 0, 0);

	// start tab watcher
	pros::Task tabWatcher_task(tabWatcher);

}

} // namespace selector

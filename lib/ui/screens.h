#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_TANK_UI = 2,
    SCREEN_ID_SOIL_UI = 3,
    _SCREEN_ID_LAST = 3
};

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *tank_ui;
    lv_obj_t *soil_ui;
    lv_obj_t *soil_page_main;
    lv_obj_t *tank_page_main;
    lv_obj_t *time_lable;
    lv_obj_t *water_bar_main;
    lv_obj_t *turbid_bar_main;
    lv_obj_t *water_value_main;
    lv_obj_t *turbid_value_main;
    lv_obj_t *soil_bar_main;
    lv_obj_t *soil_value_main;
    lv_obj_t *temp_lable;
    lv_obj_t *date_lable;
    lv_obj_t *light_sw_onoff;
    lv_obj_t *led_light_status;
    lv_obj_t *pump_led_main;
    lv_obj_t *auto_switch;
    lv_obj_t *tank_noti_main;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *agree_bt_main;
    lv_obj_t *obj2;
    lv_obj_t *soil_page_tank;
    lv_obj_t *main_page_tank;
    lv_obj_t *obj3;
    lv_obj_t *turbid_gauge_tank;
    lv_obj_t *turbid_value_tank;
    lv_obj_t *water_bar_tank;
    lv_obj_t *water_value_tank;
    lv_obj_t *pump_on_tank;
    lv_obj_t *pump_off_tank;
    lv_obj_t *obj4;
    lv_obj_t *pump_led_tank;
    lv_obj_t *tank_noti_tank;
    lv_obj_t *obj5;
    lv_obj_t *obj6;
    lv_obj_t *agree_bt_tank;
    lv_obj_t *obj7;
    lv_obj_t *tank_page_soil;
    lv_obj_t *main_page_soil;
    lv_obj_t *obj8;
    lv_obj_t *moisture_bar_soil;
    lv_obj_t *obj9;
    lv_obj_t *moisture_value_soil;
    lv_obj_t *pump_off_soil;
    lv_obj_t *obj10;
    lv_obj_t *obj11;
    lv_obj_t *pump_led_soil;
    lv_obj_t *obj12;
    lv_obj_t *pump_on_soil;
    lv_obj_t *obj13;
    lv_obj_t *obj14;
    lv_obj_t *humid_value_soil;
    lv_obj_t *tank_noti_soil;
    lv_obj_t *obj15;
    lv_obj_t *obj16;
    lv_obj_t *agree_bt_soil;
    lv_obj_t *obj17;
} objects_t;

extern objects_t objects;

typedef struct {
    lv_meter_scale_t *scale;
    lv_meter_indicator_t *indicator;
} screen_tank_ui_state_t;

extern screen_tank_ui_state_t screen_tank_ui_state;

void create_screen_main();
void tick_screen_main();

void create_screen_tank_ui();
void tick_screen_tank_ui();

void create_screen_soil_ui();
void tick_screen_soil_ui();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/
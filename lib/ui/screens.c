#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;

screen_tank_ui_state_t screen_tank_ui_state;

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

//
// Screens
//

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xe1f6d1), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // soil_page_main
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.soil_page_main = obj;
            lv_obj_set_pos(obj, 17, 188);
            lv_obj_set_size(obj, 40, 40);
            lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x61462b), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Soil");
                }
            }
        }
        {
            // tank_page_main
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.tank_page_main = obj;
            lv_obj_set_pos(obj, 17, 138);
            lv_obj_set_size(obj, 40, 40);
            lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x5f9de8), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Tank");
                }
            }
        }
        {
            // time_lable
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.time_lable = obj;
            lv_obj_set_pos(obj, 202, 14);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Ti : me : s0 AM");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 10, 14);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "So Fat So Growth farm");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 10, 44);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Date :");
        }
        {
            // water_bar_main
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.water_bar_main = obj;
            lv_obj_set_pos(obj, 150, 150);
            lv_obj_set_size(obj, 115, 9);
            lv_bar_set_value(obj, 25, LV_ANIM_OFF);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x003285), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x003285), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        }
        {
            // turbid_bar_main
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.turbid_bar_main = obj;
            lv_obj_set_pos(obj, 151, 178);
            lv_obj_set_size(obj, 114, 10);
            lv_bar_set_value(obj, 25, LV_ANIM_OFF);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3a3a3a), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 91, 146);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Level");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 76, 175);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Turbidity");
        }
        {
            // water_value_main
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.water_value_main = obj;
            lv_obj_set_pos(obj, 274, 145);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Text");
        }
        {
            // turbid_value_main
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.turbid_value_main = obj;
            lv_obj_set_pos(obj, 274, 175);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Text");
        }
        {
            // soil_bar_main
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.soil_bar_main = obj;
            lv_obj_set_pos(obj, 151, 214);
            lv_obj_set_size(obj, 114, 9);
            lv_bar_set_value(obj, 25, LV_ANIM_OFF);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xf897f9), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xf897f9), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 78, 198);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "    Soil\nMoisture");
        }
        {
            // soil_value_main
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.soil_value_main = obj;
            lv_obj_set_pos(obj, 274, 210);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Text");
        }
        {
            // temp_lable
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.temp_lable = obj;
            lv_obj_set_pos(obj, 204, 37);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "26.8 * C");
        }
        {
            // date_lable
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.date_lable = obj;
            lv_obj_set_pos(obj, 60, 44);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Text");
        }
        {
            // light_sw_onoff
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.light_sw_onoff = obj;
            lv_obj_set_pos(obj, 47, 90);
            lv_obj_set_size(obj, 41, 18);
        }
        {
            // led_light_status
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.led_light_status = obj;
            lv_obj_set_pos(obj, 118, 112);
            lv_obj_set_size(obj, 24, 26);
            lv_led_set_color(obj, lv_color_hex(0x0eff00));
            lv_led_set_brightness(obj, 255);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 10, 69);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Light Control");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 91);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "OFF");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 94, 91);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "ON");
        }
        {
            // pump_led_main
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.pump_led_main = obj;
            lv_obj_set_pos(obj, 279, 112);
            lv_obj_set_size(obj, 24, 26);
            lv_led_set_color(obj, lv_color_hex(0x75c2f6));
            lv_led_set_brightness(obj, 255);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 170, 78);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 163, 69);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Auto Farm");
        }
        {
            // Auto_switch
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.auto_switch = obj;
            lv_obj_set_pos(obj, 201, 89);
            lv_obj_set_size(obj, 41, 18);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 163, 91);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "OFF");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 248, 91);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "ON");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 11, 117);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Light Status");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 163, 117);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Pump Status");
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}

void create_screen_tank_ui() {
    screen_tank_ui_state_t *state = &screen_tank_ui_state;
    (void)state;
    lv_obj_t *obj = lv_obj_create(0);
    objects.tank_ui = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xcce0fa), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // soil_page_tank
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.soil_page_tank = obj;
            lv_obj_set_pos(obj, 17, 188);
            lv_obj_set_size(obj, 40, 40);
            lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x61462b), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Soil");
                }
            }
        }
        {
            // main_page_tank
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.main_page_tank = obj;
            lv_obj_set_pos(obj, 17, 138);
            lv_obj_set_size(obj, 40, 40);
            lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xe1f6d1), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj0 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x010101), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Main");
                }
            }
        }
        {
            // turbid_gauge_tank
            lv_obj_t *obj = lv_meter_create(parent_obj);
            objects.turbid_gauge_tank = obj;
            lv_obj_set_pos(obj, 68, 13);
            lv_obj_set_size(obj, 130, 130);
            {
                lv_meter_scale_t *scale = lv_meter_add_scale(obj);
                state->scale = scale;
                lv_meter_set_scale_ticks(obj, scale, 41, 1, 5, lv_color_hex(0x90ff00));
                lv_meter_set_scale_major_ticks(obj, scale, 8, 3, 10, lv_color_hex(0xfb00ff), 10);
                lv_meter_set_scale_range(obj, scale, 0, 100, 300, 120);
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_needle_line(obj, scale, 4, lv_color_hex(0x85ff00), -28);
                    state->indicator = indicator;
                    lv_meter_set_indicator_value(obj, indicator, 30);
                }
            }
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 7, 20);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Tubidity");
        }
        {
            // turbid_value_tank
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.turbid_value_tank = obj;
            lv_obj_set_pos(obj, 20, 53);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Text");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 21, 81);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "NTU");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 76, 165);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Water Level");
        }
        {
            // water_bar_tank
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.water_bar_tank = obj;
            lv_obj_set_pos(obj, 76, 196);
            lv_obj_set_size(obj, 182, 25);
            lv_bar_set_value(obj, 25, LV_ANIM_OFF);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x003285), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x003285), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        }
        {
            // water_value_tank
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.water_value_tank = obj;
            lv_obj_set_pos(obj, 271, 203);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Text");
        }
        {
            // pump_on_tank
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.pump_on_tank = obj;
            lv_obj_set_pos(obj, 209, 41);
            lv_obj_set_size(obj, 97, 37);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x8bfe61), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "On");
                }
            }
        }
        {
            // pump_off_tank
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.pump_off_tank = obj;
            lv_obj_set_pos(obj, 209, 90);
            lv_obj_set_size(obj, 97, 37);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj1 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Off");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 206, 17);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Pump Control");
        }
        {
            // pump_led_tank
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.pump_led_tank = obj;
            lv_obj_set_pos(obj, 268, 148);
            lv_obj_set_size(obj, 32, 32);
            lv_led_set_color(obj, lv_color_hex(0x75c2f6));
            lv_led_set_brightness(obj, 255);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 198, 148);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Pump\nStatus");
        }
    }
    
    tick_screen_tank_ui();
}

void tick_screen_tank_ui() {
    screen_tank_ui_state_t *state = &screen_tank_ui_state;
    (void)state;
}

void create_screen_soil_ui() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.soil_ui = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x61462b), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // tank_page_soil
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.tank_page_soil = obj;
            lv_obj_set_pos(obj, 17, 188);
            lv_obj_set_size(obj, 40, 40);
            lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x5f9de8), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Tank");
                }
            }
        }
        {
            // main_page_soil
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.main_page_soil = obj;
            lv_obj_set_pos(obj, 17, 138);
            lv_obj_set_size(obj, 40, 40);
            lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xe1f6d1), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj2 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x010101), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Main");
                }
            }
        }
        {
            // moisture_bar_soil
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.moisture_bar_soil = obj;
            lv_obj_set_pos(obj, 118, 13);
            lv_obj_set_size(obj, 194, 33);
            lv_bar_set_value(obj, 25, LV_ANIM_OFF);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xf897f9), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xf897f9), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj3 = obj;
            lv_obj_set_pos(obj, 10, 20);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Soil Moisture");
        }
        {
            // moisture_value_soil
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.moisture_value_soil = obj;
            lv_obj_set_pos(obj, 198, 21);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Text");
        }
        {
            // pump_off_soil
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.pump_off_soil = obj;
            lv_obj_set_pos(obj, 225, 62);
            lv_obj_set_size(obj, 78, 37);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj4 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Off");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj5 = obj;
            lv_obj_set_pos(obj, 10, 72);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Pump Control");
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 82, 111);
            lv_obj_set_size(obj, 78, 118);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pump_led_soil
                    lv_obj_t *obj = lv_led_create(parent_obj);
                    objects.pump_led_soil = obj;
                    lv_obj_set_pos(obj, 7, 46);
                    lv_obj_set_size(obj, 32, 32);
                    lv_led_set_color(obj, lv_color_hex(0x75c2f6));
                    lv_led_set_brightness(obj, 255);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj6 = obj;
                    lv_obj_set_pos(obj, 1, -4);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Pump\nStatus");
                }
            }
        }
        {
            // pump_on_soil
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.pump_on_soil = obj;
            lv_obj_set_pos(obj, 136, 62);
            lv_obj_set_size(obj, 78, 37);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x8bfe61), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "On");
                }
            }
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj7 = obj;
            lv_obj_set_pos(obj, 177, 111);
            lv_obj_set_size(obj, 126, 118);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xe0f7ff), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj8 = obj;
                    lv_obj_set_pos(obj, 14, 4);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Humidity");
                }
            }
        }
        {
            // Humid_value_soil
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.humid_value_soil = obj;
            lv_obj_set_pos(obj, 206, 167);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "0 %");
        }
    }
    
    tick_screen_soil_ui();
}

void tick_screen_soil_ui() {
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_tank_ui,
    tick_screen_soil_ui,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 3) {
        tick_screen_funcs[screen_index]();
    }
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen(screenId - 1);
}

//
// Fonts
//

ext_font_desc_t fonts[] = {
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
// Color themes
//

uint32_t active_theme_index = 0;

//
//
//

void create_screens() {

// Set default LVGL theme
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    // Initialize screens
    // Create screens
    create_screen_main();
    create_screen_tank_ui();
    create_screen_soil_ui();
}
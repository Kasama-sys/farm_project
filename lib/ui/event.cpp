#include "event.h"
#include <Arduino.h>
#include <time.h>

#define WATER_MIN_PERCENT 40     
#define TURBID_MAX_PERCENT 60     
#define ECHO_TIMEOUT_US 30000 

static bool waterAlertShown = false ; 

#define TURBID_CLEAR_RAW  700   // measured: clean water
#define TURBID_DIRTY_RAW  600   // measured: dirt-stirred water

#define SOIL_DRY_RAW 2500   // CALIBRATE: raw ADC value in dry air 2550
#define SOIL_WET_RAW 1000  // CALIBRATE: raw ADC value in water 750
#define SOIL_TRIGGER_PERCENT 20

#define LUX_DARK_THRESHOLD 50     // lights turn on automatically below this lux

#define MANUAL_OVERRIDE_MS 15000

TankSafety tankSafety = { false, false, 0, 0 } ;

bool isAutoMode = false ;

static void setWaterAlertVisible(bool visible) {
    if (visible) {
        lv_obj_clear_flag(objects.tank_noti_main, LV_OBJ_FLAG_HIDDEN) ;
        lv_obj_clear_flag(objects.tank_noti_tank, LV_OBJ_FLAG_HIDDEN) ;
        lv_obj_clear_flag(objects.tank_noti_soil, LV_OBJ_FLAG_HIDDEN) ;
    } else {
        lv_obj_add_flag(objects.tank_noti_main, LV_OBJ_FLAG_HIDDEN) ;
        lv_obj_add_flag(objects.tank_noti_tank, LV_OBJ_FLAG_HIDDEN) ;
        lv_obj_add_flag(objects.tank_noti_soil, LV_OBJ_FLAG_HIDDEN) ;
    }
}

static void setPumpLeds(bool on) {
    if (on) {
        lv_led_on(objects.pump_led_main) ;
        lv_led_on(objects.pump_led_tank) ;
        lv_led_on(objects.pump_led_soil) ;
    } else {
        lv_led_off(objects.pump_led_main) ;
        lv_led_off(objects.pump_led_tank) ;
        lv_led_off(objects.pump_led_soil) ;
    }
}

// -----------------------------------------------------------------------
// DATE/TIME: use epoch
// -----------------------------------------------------------------------
void datetime_handler(lv_timer_t *timer) {
    LV_UNUSED(timer) ;

    struct tm timeinfo ;
    if (!getLocalTime(&timeinfo, 100)) {
        lv_label_set_text(objects.time_lable, "No time set") ;
        return ;
    }

    char timeBuf[16] ;
    char dateBuf[24] ;

    strftime(timeBuf, sizeof(timeBuf), "%I:%M:%S %p", &timeinfo) ;
    strftime(dateBuf, sizeof(dateBuf), "%a, %d %b %Y", &timeinfo) ;

    lv_label_set_text(objects.time_lable, timeBuf) ;
    lv_label_set_text(objects.date_lable, dateBuf) ;
}

// -----------------------------------------------------------------------
// TANK: water level + turbidity also the pump safety gate
// -----------------------------------------------------------------------
void tank_handler(lv_timer_t *timer) {
    Tank_pin *pin = (Tank_pin *)timer->user_data ;
    screen_tank_ui_state_t *state = &screen_tank_ui_state ;

    long echo_read, echo_value ;
    int  waterPercent, turbidPercent ;
    const int Max_depth  = 14 ;
    const int Max_turval = 4095 ;

    digitalWrite(*pin->Trig, LOW) ;
    delayMicroseconds(2) ;
    digitalWrite(*pin->Trig, HIGH) ;
    delayMicroseconds(10) ;
    digitalWrite(*pin->Trig, LOW) ;
    echo_read = pulseIn(*pin->Echo, HIGH , ECHO_TIMEOUT_US) ;

    echo_value   = (echo_read * 0.029) / 2 ;
    waterPercent = ((Max_depth - echo_value) * 100) / Max_depth ;
    waterPercent = constrain(waterPercent, 0, 100) ;

    int turbid_read = analogRead(*pin->Turbid) ;
    turbidPercent = map(turbid_read, TURBID_CLEAR_RAW, TURBID_DIRTY_RAW, 0, 100) ;
    turbidPercent = constrain(turbidPercent, 0, 100) ;

    if (waterPercent < WATER_MIN_PERCENT && !waterAlertShown) {
        setWaterAlertVisible(true) ;
        waterAlertShown = true ;   // mark as warned — never shows again this boot
    }

    char depth[8], turb[8] ;
    sprintf(depth, "%d%%", waterPercent) ;
    sprintf(turb,  "%d", turbidPercent) ;

    lv_bar_set_value(objects.water_bar_main, waterPercent, LV_ANIM_OFF) ;
    lv_bar_set_value(objects.water_bar_tank, waterPercent, LV_ANIM_OFF) ;
    lv_label_set_text(objects.water_value_main, depth) ;
    lv_label_set_text(objects.water_value_tank, depth) ;

    lv_bar_set_value(objects.turbid_bar_main, turbidPercent, LV_ANIM_OFF) ;
    lv_label_set_text(objects.turbid_value_main, turb) ;
    lv_label_set_text(objects.turbid_value_tank, turb) ;
    lv_meter_set_indicator_value(objects.turbid_gauge_tank, state->indicator, turbidPercent) ;

    tankSafety.waterPercent  = waterPercent ;
    tankSafety.turbidPercent = turbidPercent ;
    tankSafety.level_w0rk  = waterPercent  > WATER_MIN_PERCENT ;
    tankSafety.turbid_w0rk = turbidPercent < TURBID_MAX_PERCENT ;

    if (!tankSafety.level_w0rk || !tankSafety.turbid_w0rk) {
        digitalWrite(*pin->Pump, HIGH) ;
        setPumpLeds(false) ;
        Serial.println(">>> Pump forced OFF by safety gate <<<") ;
    }
}

// -----------------------------------------------------------------------
// SOIL: capacitive moisture — drives automatic irrigation trigger
// -----------------------------------------------------------------------
void soil_handler(lv_timer_t *timer) {
    Soil_pin *pin = (Soil_pin *)timer->user_data ;

    int raw = analogRead(*pin->Soil) ;
    int moisturePercent = map(raw, SOIL_DRY_RAW, SOIL_WET_RAW, 0, 100) ;
    moisturePercent = constrain(moisturePercent, 0, 100) ;

    char buf[8] ;
    sprintf(buf, "%d%%", moisturePercent) ;
    lv_bar_set_value(objects.soil_bar_main , moisturePercent , LV_ANIM_OFF) ;
    lv_bar_set_value(objects.moisture_bar_soil , moisturePercent , LV_ANIM_OFF) ;
    lv_label_set_text(objects.soil_value_main , buf) ;
    lv_label_set_text(objects.moisture_value_soil, buf) ;

    if (!isAutoMode) return ;   // Manual mode: buttons only, skip automation entirely

    bool isDry = moisturePercent < SOIL_TRIGGER_PERCENT ;

    if (isDry && tankSafety.level_w0rk && tankSafety.turbid_w0rk) {
        digitalWrite(*pin->Pump, LOW) ;   // ON
        setPumpLeds(true) ;
    } else if (!isDry) {
        digitalWrite(*pin->Pump, HIGH) ;  // OFF
        setPumpLeds(false) ;
    }
    // isDry but unsafe: tank_handler already forces pump off each tick.
}

// -----------------------------------------------------------------------
// LIGHT: BH1750 lux — automatic day/night control + manual switch
// -----------------------------------------------------------------------
void light_handler(lv_timer_t *timer) {
    Light_ctx *ctx = (Light_ctx *)timer->user_data ;
    float lux = ctx->meter->readLightLevel() ;

    if (!isAutoMode) return ;

    bool shouldBeOn = lux < LUX_DARK_THRESHOLD ;

    digitalWrite(*ctx->pin, shouldBeOn ? HIGH : LOW) ;   // flipped to match manual switch's polarity

    if (shouldBeOn) lv_led_on(objects.led_light_status) ;
    else            lv_led_off(objects.led_light_status) ;

    if (shouldBeOn) lv_obj_add_state(objects.light_sw_onoff, LV_STATE_CHECKED) ;
    else            lv_obj_clear_state(objects.light_sw_onoff, LV_STATE_CHECKED) ;
}

// -----------------------------------------------------------------------
// DHT22: ambient temperature + humidity
// -----------------------------------------------------------------------
void dht_handler(lv_timer_t *timer) {
    DHT *sensor = (DHT *)timer->user_data ;
    float t = sensor->readTemperature() ;
    float h = sensor->readHumidity() ;

    if (isnan(t) || isnan(h)) {
        return ; // bad read this cycle — keep showing the last good value
    }

    char tbuf[16], hbuf[16] ;
    sprintf(tbuf, "%.1f * C", t) ;
    sprintf(hbuf, "%.0f%%", h) ;

    lv_label_set_text(objects.temp_lable, tbuf) ;
    lv_label_set_text(objects.humid_value_soil, hbuf) ;
}

// -----------------------------------------------------------------------
// Button / navigation / manual override events
// -----------------------------------------------------------------------
void event_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e) ;
    lv_obj_t *obj = lv_event_get_target(e) ;
    int *pPump = (int *)lv_event_get_user_data(e) ;

    if ((obj == objects.tank_page_main || obj == objects.tank_page_soil) && code == LV_EVENT_CLICKED) {
        lv_scr_load(objects.tank_ui) ;
    } else if ((obj == objects.soil_page_main || obj == objects.soil_page_tank) && code == LV_EVENT_CLICKED) {
        lv_scr_load(objects.soil_ui) ;
    } else if ((obj == objects.main_page_tank || obj == objects.main_page_soil) && code == LV_EVENT_CLICKED) {
        lv_scr_load(objects.main) ;
    } else if ((obj == objects.pump_on_tank || obj == objects.pump_on_soil) && code == LV_EVENT_CLICKED) {
        if (tankSafety.level_w0rk && tankSafety.turbid_w0rk) {
            digitalWrite((gpio_num_t)*pPump, LOW) ;   // ON
            setPumpLeds(true) ;
        }
    } else if ((obj == objects.pump_off_tank || obj == objects.pump_off_soil) && code == LV_EVENT_CLICKED) {
        digitalWrite((gpio_num_t)*pPump, HIGH) ;   // OFF
        setPumpLeds(false) ;
    } else if (obj == objects.light_sw_onoff && code == LV_EVENT_VALUE_CHANGED) {
        int *pLight = (int *)lv_event_get_user_data(e) ;
        bool on = lv_obj_has_state(obj, LV_STATE_CHECKED) ;
        digitalWrite(*pLight, on ? HIGH : LOW) ;
        if (on) lv_led_on(objects.led_light_status) ;
        else lv_led_off(objects.led_light_status) ;
    } else if (obj == objects.auto_switch && code == LV_EVENT_VALUE_CHANGED) {
        isAutoMode = lv_obj_has_state(obj, LV_STATE_CHECKED) ;
        Serial.println(isAutoMode ? "Mode: AUTO" : "Mode: MANUAL") ;
    } else if ((obj == objects.agree_bt_main || obj == objects.agree_bt_tank || obj == objects.agree_bt_soil) && code == LV_EVENT_CLICKED) {
        setWaterAlertVisible(false) ;
    }
}

/*#include "event.h"
#include <Arduino.h>

#define WATER_MIN_PERCENT 20     
#define TURBID_MAX_PERCENT 70     
#define ECHO_TIMEOUT_US 30000 

#define SOIL_DRY_RAW 3000
#define SOIL_WET_RAW 1200
#define SOIL_TRIGGER_PERCENT 30

TankSafety tankSafety = { false, false, 0, 0 } ;

void turbid_gauge_update(lv_timer_t *timer) {
    LV_UNUSED(timer) ;
    int turbid_read = analogRead(33) ;
    lv_meter_set_indicator_value(objects.turbid_gauge_tank , turbid_read) ;
}

void tank_handler(lv_timer_t *timer) {
    Tank_pin *pin = (Tank_pin *)timer->user_data ;

    //Turbid ยิ่งใสค่ายิ่งเยอะ
    long echo_read , echo_value ;
    int water_tank , Max_depth , Max_turval, turbid_read , turbid_value ;
    Max_depth = 16 ;
    Max_turval = 4095 ;

    digitalWrite(*pin->Trig, LOW) ;
    delayMicroseconds(2) ;
    digitalWrite(*pin->Trig ,HIGH) ;
    delayMicroseconds(10) ;
    digitalWrite(*pin->Trig, LOW) ;
    echo_read = pulseIn(*pin->Echo , HIGH) ;
    turbid_read = analogRead(*pin->Turbid) ;
    
    echo_value = (echo_read * 0.034) / 2 ;
    water_tank = ((Max_depth - echo_value)*100)/Max_depth ;
    turbid_value = ((Max_turval - turbid_read)*100)/Max_turval ;

    water_tank = constrain(water_tank , 0 , 100) ;
    turbid_value = constrain(turbid_value , 0 , 100) ;

    char depth[8] ;
    char turb[8] ;
    sprintf(depth , "%d%%" , water_tank) ;
    sprintf(turb , "%d%%" , turbid_value) ;

    lv_bar_set_value(objects.water_bar_main , water_tank , LV_ANIM_OFF) ;
    lv_bar_set_value(objects.water_bar_tank , water_tank , LV_ANIM_OFF) ;
    lv_label_set_text(objects.water_value_main , depth) ;
    lv_label_set_text(objects.water_value_tank , depth) ;
    
    lv_bar_set_value(objects.turbid_bar_main , turbid_value , LV_ANIM_OFF) ;
    // lv_bar_set_value(objects.turbid) ;

    if (water_tank < 10) {
        digitalWrite(*pin->Pump , LOW) ;
        lv_led_toggle(objects.pump_led_main) ;
        lv_led_toggle(objects.pump_led_tank) ;
        lv_led_toggle(objects.pump_led_soil) ;
    } else if (water_tank > 10) {
        digitalWrite((gpio_num_t)*pPump , High) ;
    }
}

void event_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e) ;
    lv_obj_t *obj = lv_event_get_target(e) ;
    int *pPump = (int *)lv_event_get_user_data(e) ;

    if ((obj == objects.tank_page_main || obj == objects.tank_page_soil) && code == LV_EVENT_CLICKED) {
        lv_scr_load(objects.tank_ui) ;
    } else if ((obj == objects.soil_page_main || obj == objects.soil_page_tank) && code == LV_EVENT_CLICKED) {
        lv_scr_load(objects.soil_ui) ;
    } else if ((obj == objects.main_page_tank || obj == objects.main_page_soil) && code == LV_EVENT_CLICKED) {
        lv_scr_load(objects.main) ;
    } else if ((obj == objects.pump_on_tank || obj == objects.pump_on_soil) && code == LV_EVENT_CLICKED) {
        digitalWrite((gpio_num_t)*pPump , HIGH) ;
        lv_led_on(objects.pump_led_main) ;
        lv_led_on(objects.pump_led_tank) ;
        lv_led_on(objects.pump_led_soil) ;
    } else if ((obj == objects.pump_off_tank || obj == objects.pump_off_soil) && code == LV_EVENT_CLICKED) {
        digitalWrite((gpio_num_t)*pPump , LOW) ;
        lv_led_off(objects.pump_led_main) ;
        lv_led_off(objects.pump_led_tank) ;
        lv_led_off(objects.pump_led_soil) ;
    }
}*/

// void in_de_spinbox(lv_event_t *s) {
//     lv_event_code_t code = lv_event_get_code(s) ;
//     lv_obj_t *obj = lv_event_get_target(s) ;
//     int *pLED = (int *)lv_event_get_user_data(s) ;
    
//     if (obj == objects.incrementpwm && code == LV_EVENT_LONG_PRESSED) {
//         lv_spinbox_increment(objects.pwmval);
//     }else if (obj == objects.decrementpwm && code == LV_EVENT_LONG_PRESSED) {
//         lv_spinbox_decrement(objects.pwmval);
//     }else if (obj == objects.onpwm_sw && code == LV_EVENT_VALUE_CHANGED) {
//         int sw_stt = lv_obj_has_state(objects.onpwm_sw , LV_STATE_CHECKED) ;
//         if (sw_stt == 1) {
//             int spinval = lv_spinbox_get_value(objects.pwmval) ;
//             analogWrite((gpio_num_t)*pLED , spinval) ;
//             Serial.printf("PWMVAL : %d\n" , spinval) ;
//         }else {
//             analogWrite((gpio_num_t)*pLED , 0);
//         }
//     }
// }

// void fan_on_off(lv_timer_t *timer) {
//     int *pSW = (int *)timer->user_data ;
//     int sw_state = digitalRead((gpio_num_t)*pSW) ;

//     if (sw_state == 0) {
//         lv_led_on(objects.fan_led) ;
//     } else {
//         lv_led_off(objects.fan_led) ;
//     }
// }
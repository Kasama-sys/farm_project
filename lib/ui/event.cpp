#include "event.h"
#include <Arduino.h>
#include <time.h>

#define WATER_MIN_PERCENT 15
#define TURBID_MAX_PERCENT 70
#define ECHO_TIMEOUT_US 30000

#define SOIL_DRY_RAW 2550
#define SOIL_WET_RAW 1550

// ปั๊มจะเริ่มทำงานเมื่อความชื้นต่ำกว่าค่านี้
// และจะหยุดเมื่อความชื้นถึงค่านี้
#define SOIL_TRIGGER_PERCENT 20

#define LUX_DARK_THRESHOLD 50

TankSafety tankSafety = { false, false, 0, 0 };

// =====================================================
// CONTROL STATES
// =====================================================

static bool autoFarm = false;
static bool lightManualState = false;

// =====================================================
// PUMP LED
// =====================================================

static void setPumpLeds(bool on)
{
    if (on) {
        lv_led_on(objects.pump_led_main);
        lv_led_on(objects.pump_led_tank);
        lv_led_on(objects.pump_led_soil);
    } else {
        lv_led_off(objects.pump_led_main);
        lv_led_off(objects.pump_led_tank);
        lv_led_off(objects.pump_led_soil);
    }
}

// =====================================================
// DATE / TIME
// =====================================================

void datetime_handler(lv_timer_t *timer)
{
    LV_UNUSED(timer);

    struct tm timeinfo;

    if (!getLocalTime(&timeinfo, 100)) {
        lv_label_set_text(objects.time_lable, "No time set");
        return;
    }

    char timeBuf[16];
    char dateBuf[24];

    strftime(timeBuf, sizeof(timeBuf), "%I:%M:%S %p", &timeinfo);
    strftime(dateBuf, sizeof(dateBuf), "%a, %d %b %Y", &timeinfo);

    lv_label_set_text(objects.time_lable, timeBuf);
    lv_label_set_text(objects.date_lable, dateBuf);
}

// =====================================================
// TANK
// Water level + turbidity
//
// NOTE:
// Turbidity DOES NOT control the pump.
// Water level is still used as pump safety.
// =====================================================

void tank_handler(lv_timer_t *timer)
{
    Tank_pin *pin = (Tank_pin *)timer->user_data;
    screen_tank_ui_state_t *state = &screen_tank_ui_state;

    long echo_read;
    long echo_value;

    int waterPercent;
    int turbidPercent;

    const int Max_depth = 16;
    const int Max_turval = 4095;

    digitalWrite(*pin->Trig, LOW);
    delayMicroseconds(2);

    digitalWrite(*pin->Trig, HIGH);
    delayMicroseconds(10);

    digitalWrite(*pin->Trig, LOW);

    echo_read = pulseIn(*pin->Echo, HIGH, ECHO_TIMEOUT_US);

    echo_value = (echo_read * 0.029) / 2;

    waterPercent = ((Max_depth - echo_value) * 100) / Max_depth;
    waterPercent = constrain(waterPercent, 0, 100);

    int turbid_read = analogRead(*pin->Turbid);

    turbidPercent =
        ((Max_turval - turbid_read) * 100) / Max_turval;

    turbidPercent = constrain(turbidPercent, 0, 100);

    // =================================================
    // DISPLAY
    // =================================================

    char depth[8];
    char turb[8];

    sprintf(depth, "%d%%", waterPercent);
    sprintf(turb, "%d", turbidPercent);

    lv_bar_set_value(
        objects.water_bar_main,
        waterPercent,
        LV_ANIM_OFF
    );

    lv_bar_set_value(
        objects.water_bar_tank,
        waterPercent,
        LV_ANIM_OFF
    );

    lv_label_set_text(
        objects.water_value_main,
        depth
    );

    lv_label_set_text(
        objects.water_value_tank,
        depth
    );

    lv_bar_set_value(
        objects.turbid_bar_main,
        turbidPercent,
        LV_ANIM_OFF
    );

    lv_label_set_text(
        objects.turbid_value_main,
        turb
    );

    lv_label_set_text(
        objects.turbid_value_tank,
        turb
    );

    lv_meter_set_indicator_value(
        objects.turbid_gauge_tank,
        state->indicator,
        turbidPercent
    );

    // =================================================
    // SAFETY STATUS
    // =================================================

    tankSafety.waterPercent = waterPercent;
    tankSafety.turbidPercent = turbidPercent;

    // น้ำต้องมากกว่า 15%
    tankSafety.level_w0rk =
        waterPercent > WATER_MIN_PERCENT;

    // ความขุ่นเก็บสถานะไว้แสดงผล
    // แต่จะไม่เอาไปควบคุมปั๊ม
    tankSafety.turbid_w0rk =
        turbidPercent < TURBID_MAX_PERCENT;

    // =================================================
    // PUMP SAFETY
    //
    // ใช้เฉพาะระดับน้ำ
    //
    // ถ้าน้ำต่ำกว่า 15%
    // -> ปั๊ม OFF
    //
    // ความขุ่นไม่มีผลกับปั๊ม
    // =================================================

    if (!tankSafety.level_w0rk) {

        digitalWrite(*pin->Pump, LOW);
        setPumpLeds(false);

        Serial.println("Pump OFF: WATER LEVEL LOW");
    }
}

// =====================================================
// SOIL
// =====================================================

void soil_handler(lv_timer_t *timer)
{
    Soil_pin *pin = (Soil_pin *)timer->user_data;

    int raw = analogRead(*pin->Soil);

    int moisturePercent =
        map(
            raw,
            SOIL_DRY_RAW,
            SOIL_WET_RAW,
            0,
            100
        );

    moisturePercent =
        constrain(moisturePercent, 0, 100);

    // =================================================
    // DISPLAY
    // =================================================

    char buf[8];

    sprintf(buf, "%d%%", moisturePercent);

    lv_bar_set_value(
        objects.soil_bar_main,
        moisturePercent,
        LV_ANIM_OFF
    );

    lv_bar_set_value(
        objects.moisture_bar_soil,
        moisturePercent,
        LV_ANIM_OFF
    );

    lv_label_set_text(
        objects.soil_value_main,
        buf
    );

    lv_label_set_text(
        objects.moisture_value_soil,
        buf
    );

    // =================================================
    // AUTO FARM
    // =================================================

    if (autoFarm) {

        // =============================================
        // ความชื้นต่ำกว่า 30%
        // -> ปั๊มทำงาน
        //
        // ปั๊มจะทำงานต่อไปจนกว่า
        // moisturePercent >= 30%
        // =============================================

        if (moisturePercent < SOIL_TRIGGER_PERCENT) {

            // ตรวจเฉพาะระดับน้ำ
            // ไม่ตรวจ turbidity
            if (tankSafety.level_w0rk) {

                digitalWrite(*pin->Pump, HIGH);
                setPumpLeds(true);

            } else {

                digitalWrite(*pin->Pump, LOW);
                setPumpLeds(false);

                Serial.println(
                    "Pump OFF: WATER LEVEL LOW"
                );
            }

        }

        // =============================================
        // ความชื้นถึง 30% แล้ว
        // -> หยุดปั๊ม
        // =============================================

        else {

            digitalWrite(*pin->Pump, LOW);
            setPumpLeds(false);
        }
    }

    // =================================================
    // MANUAL MODE
    //
    // ไม่ทำอะไรกับปั๊ม
    // ปุ่ม Pump ON / OFF เป็นผู้ควบคุม
    // =================================================
}

// =====================================================
// LIGHT
// =====================================================

void light_handler(lv_timer_t *timer)
{
    Light_ctx *ctx =
        (Light_ctx *)timer->user_data;

    float lux =
        ctx->meter->readLightLevel();

    bool shouldBeOn;

    // =================================================
    // AUTO FARM
    // =================================================

    if (autoFarm) {

        shouldBeOn =
            lux < LUX_DARK_THRESHOLD;

    }

    // =================================================
    // MANUAL
    // =================================================

    else {

        shouldBeOn =
            lightManualState;
    }

    digitalWrite(
        *ctx->pin,
        shouldBeOn ? HIGH : LOW
    );

    if (shouldBeOn)
        lv_led_on(objects.led_light_status);
    else
        lv_led_off(objects.led_light_status);

    // Update switch

    if (shouldBeOn)
        lv_obj_add_state(
            objects.light_sw_onoff,
            LV_STATE_CHECKED
        );
    else
        lv_obj_clear_state(
            objects.light_sw_onoff,
            LV_STATE_CHECKED
        );
}

// =====================================================
// DHT22
// =====================================================

void dht_handler(lv_timer_t *timer)
{
    DHT *sensor =
        (DHT *)timer->user_data;

    float t =
        sensor->readTemperature();

    float h =
        sensor->readHumidity();

    if (isnan(t) || isnan(h)) {
        return;
    }

    char tbuf[16];
    char hbuf[16];

    sprintf(tbuf, "%.1f * C", t);
    sprintf(hbuf, "%.0f%%", h);

    lv_label_set_text(
        objects.temp_lable,
        tbuf
    );

    lv_label_set_text(
        objects.humid_value_soil,
        hbuf
    );
}

// =====================================================
// EVENT HANDLER
// =====================================================

void event_handler(lv_event_t *e)
{
    lv_event_code_t code =
        lv_event_get_code(e);

    lv_obj_t *obj =
        lv_event_get_target(e);

    int *pPump =
        (int *)lv_event_get_user_data(e);

    // =================================================
    // PAGE NAVIGATION
    // =================================================

    if (
        (obj == objects.tank_page_main ||
         obj == objects.tank_page_soil) &&
        code == LV_EVENT_CLICKED
    ) {

        lv_scr_load(objects.tank_ui);

    }

    else if (
        (obj == objects.soil_page_main ||
         obj == objects.soil_page_tank) &&
        code == LV_EVENT_CLICKED
    ) {

        lv_scr_load(objects.soil_ui);

    }

    else if (
        (obj == objects.main_page_tank ||
         obj == objects.main_page_soil) &&
        code == LV_EVENT_CLICKED
    ) {

        lv_scr_load(objects.main);
    }

    // =================================================
    // PUMP ON
    // =================================================

    else if (
        (obj == objects.pump_on_tank ||
         obj == objects.pump_on_soil) &&
        code == LV_EVENT_CLICKED
    ) {

        // Auto Farm ON
        // Manual button ไม่มีผล

        if (autoFarm) {

            Serial.println(
                "Pump ON blocked: AUTO FARM"
            );

            return;
        }

        // Manual mode
        // ตรวจเฉพาะระดับน้ำ

        if (tankSafety.level_w0rk) {

            digitalWrite(
                (gpio_num_t)*pPump,
                HIGH
            );

            setPumpLeds(true);

            Serial.println(
                "Pump MANUAL ON"
            );

        } else {

            Serial.println(
                "Pump ON blocked: WATER LEVEL LOW"
            );
        }
    }

    // =================================================
    // PUMP OFF
    // =================================================

    else if (
        (obj == objects.pump_off_tank ||
         obj == objects.pump_off_soil) &&
        code == LV_EVENT_CLICKED
    ) {

        if (autoFarm) {

            Serial.println(
                "Pump OFF blocked: AUTO FARM"
            );

            return;
        }

        digitalWrite(
            (gpio_num_t)*pPump,
            LOW
        );

        setPumpLeds(false);

        Serial.println(
            "Pump MANUAL OFF"
        );
    }

    // =================================================
    // AUTO FARM SWITCH
    // =================================================

    else if (
        obj == objects.auto_switch &&
        code == LV_EVENT_VALUE_CHANGED
    ) {

        autoFarm =
            lv_obj_has_state(
                obj,
                LV_STATE_CHECKED
            );

        if (autoFarm) {

            Serial.println(
                "AUTO FARM: ON"
            );

        } else {

            Serial.println(
                "AUTO FARM: OFF"
            );

            // ออกจาก Auto -> หยุดปั๊ม
            // จากนั้นผู้ใช้ควบคุมเอง

            digitalWrite(
                (gpio_num_t)*pPump,
                LOW
            );

            setPumpLeds(false);
        }
    }

    // =================================================
    // LIGHT SWITCH
    // =================================================

    else if (
        obj == objects.light_sw_onoff &&
        code == LV_EVENT_VALUE_CHANGED
    ) {

        if (!autoFarm) {

            lightManualState =
                lv_obj_has_state(
                    obj,
                    LV_STATE_CHECKED
                );

            Serial.print("Manual Light: ");

            if (lightManualState)
                Serial.println("ON");
            else
                Serial.println("OFF");
        }
    }
}
/*void turbid_gauge_update(lv_timer_t *timer) {
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
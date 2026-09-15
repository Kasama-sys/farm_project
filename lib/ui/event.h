/*#ifndef EVENT_H
#define EVENT_H
#include <lvgl.h>
#include "ui.h"
#if defined(EEZ_FOR_LVGL)
#include <eez/flow/lvgl_api.h>
#endif
#if !defined(EEZ_FOR_LVGL)
#include "screens.h"
#endif

typedef struct {
    int *Pump ;
    int *Trig ;
    int *Echo ;
    int *Turbid ;
} Tank_pin ;

typedef struct {
    int *Soil ;
    int *Pump ;
} Soil_pin ;

typedef struct {
    BH1750 *meter ;
    int *pin ;
} Light_ctx ;

typedef struct {
    bool level_w0rk ;
    bool turbid_w0rk ;
    int  waterPercent ;
    int  turbidPercent ;
} TankSafety ;

extern TankSafety tankSafety ;

#ifdef __cplusplus
extern "C"
{
#endif

    void event_handler(lv_event_t *e) ;
    void tank_handler(lv_timer_t *timer) ;
    void soil_handler(lv_timer_t *timer) ;
    void light_handler(lv_timer_t *timer) ;
    void dht_handler(lv_timer_t *timer) ;

 #ifdef __cplusplus
}
#endif
#endif*/

#ifndef EVENT_H
#define EVENT_H
#include <lvgl.h>
#include "ui.h"
#if defined(EEZ_FOR_LVGL)
#include <eez/flow/lvgl_api.h>
#endif
#if !defined(EEZ_FOR_LVGL)
#include "screens.h"
#endif

#include <DHT.h>
#include <BH1750.h>

typedef struct {
    int *Pump ;
    int *Trig ;
    int *Echo ;
    int *Turbid ;
} Tank_pin ;

typedef struct {
    int *Soil ;
    int *Pump ;
} Soil_pin ;

typedef struct {
    BH1750 *meter ;
    int *pin ;
} Light_ctx ;

typedef struct {
    bool level_w0rk ;
    bool turbid_w0rk ;
    int  waterPercent ;
    int  turbidPercent ;
} TankSafety ;

extern TankSafety tankSafety ;
extern bool isAutoMode ;

#ifdef __cplusplus
extern "C"
{
#endif

    void event_handler(lv_event_t *e) ;
    void tank_handler(lv_timer_t *timer) ;
    void soil_handler(lv_timer_t *timer) ;
    void light_handler(lv_timer_t *timer) ;
    void dht_handler(lv_timer_t *timer) ;
    void datetime_handler(lv_timer_t *timer) ;

#ifdef __cplusplus
}
#endif
#endif
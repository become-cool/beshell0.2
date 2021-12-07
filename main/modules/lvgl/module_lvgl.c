#include "module_lvgl.h"
#include "display.h"
#include "widgets.h"
#include "lvgl.h"
#include "lv_conf.h"
#include "utils.h"
#include "cutils.h"

#ifndef SIMULATION
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#else
#include <signal.h>
#include <sys/time.h>
#endif


bool lv_has_inited = false ;
#define LV_TICK_PERIOD_MS 1



void lv_tick_task(void *arg) {
    (void) arg;
    lv_tick_inc(LV_TICK_PERIOD_MS);
}

#ifdef SIMULATION
struct sigaction tact;
struct itimerval value;
void sig_alm_handler(int sig_num) {
    lv_tick_inc(LV_TICK_PERIOD_MS);
}
#endif


void init_lvgl() {
    vlgl_js_display_init() ;
    vlgl_js_widgets_init() ;
}

void require_module_lvgl(JSContext *ctx) {
    if(!lv_has_inited) {
        lv_init();
        
#ifndef SIMULATION
    // lvgl 时钟
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "lvgl"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

#else 


    tact.sa_handler = sig_alm_handler;
    tact.sa_flags = SA_RESTART ;  // SA_RESTART 在 select() 时遇到中断发生，在中断完成后自动恢复
    sigemptyset(&tact.sa_mask);
    sigaction(SIGALRM, &tact, NULL);
    
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = 1000 * LV_TICK_PERIOD_MS;
    value.it_interval = value.it_value;
    setitimer(ITIMER_REAL, &value, NULL);

#endif
        
        lv_has_inited = true ;
    }

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;
    JSValue lvgl = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "lvgl", lvgl);  

    require_vlgl_js_display(ctx, lvgl) ;
    require_vlgl_js_widgets(ctx, lvgl) ;

    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx, beapi);
}

void lvgl_loop(JSContext *ctx)  {
    if(lv_has_inited) {
        lv_task_handler() ;
    }
}

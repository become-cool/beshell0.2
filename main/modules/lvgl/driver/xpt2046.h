/**
 * @file XPT2046.h
 *
 */

#ifndef XPT2046_H
#define XPT2046_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>
#include <stdbool.h>
#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

/*********************
 *      DEFINES
 *********************/
#define XPT2046_IRQ -1

#define XPT2046_AVG             4
#define XPT2046_X_MIN           200
#define XPT2046_Y_MIN           120
#define XPT2046_X_MAX           1900
#define XPT2046_Y_MAX           1900
#define XPT2046_X_INV           1
#define XPT2046_Y_INV           0
#define XPT2046_XY_SWAP		    0
#define XPT2046_TOUCH_THRESHOLD 400 // Threshold for touch detection
#define XPT2046_TOUCH_IRQ       0
#define XPT2046_TOUCH_IRQ_PRESS 0
#define XPT2046_TOUCH_PRESS     1

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void xpt2046_init(void);
bool xpt2046_read(lv_indev_drv_t * drv, lv_indev_data_t * data);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XPT2046_H */

#ifndef __INPUT
#define __INPUT

#include <stdint.h>

#define JOYPAD1_BTNS 0
#define JOYPAD1_BTN_UP 0
#define JOYPAD1_BTN_DOWN 1
#define JOYPAD1_BTN_LEFT 2
#define JOYPAD1_BTN_RIGHT 3
#define JOYPAD1_BTN_A 4
#define JOYPAD1_BTN_B 5
#define JOYPAD1_BTN_X 6
#define JOYPAD1_BTN_Y 7
#define JOYPAD1_BTN_START 8
#define JOYPAD1_BTN_SELECT 9
#define JOYPAD1_BTNS_END 9

#define JOYPAD2_BTNS 10
#define JOYPAD2_BTN_UP 10
#define JOYPAD2_BTN_DOWN 11
#define JOYPAD2_BTN_LEFT 12
#define JOYPAD2_BTN_RIGHT 13
#define JOYPAD2_BTN_A 14
#define JOYPAD2_BTN_B 15
#define JOYPAD2_BTN_X 16
#define JOYPAD2_BTN_Y 17
#define JOYPAD2_BTN_START 18
#define JOYPAD2_BTN_SELECT 19
#define JOYPAD2_BTNS_END 19


typedef struct OuputGPIO {
	uint8_t pin ;
	uint8_t value ;
} ;
typedef struct InputGPIO {
	uint8_t btn ;
	uint8_t pin ;
	uint8_t pull ;
    uint8_t pressdown ;
    uint8_t changed ;
} ;

void init_input() ;
struct InputGPIO * loop_input() ;
struct InputGPIO * input_pins() ;
void test_input() ;

uint8_t btn_has_click(uint8_t btn) ;

#endif
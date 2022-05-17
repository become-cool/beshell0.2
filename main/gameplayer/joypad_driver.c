#include "joypad_driver.h"
#include "driver/i2c.h"
#include <driver/gpio.h>
#include "config.h"

#define JOY_FREQ 100000
#define JOY1_SDA 22
#define JOY1_SCL 23
#define JOY1_ADDR 0x40
#define JOY2_ADDR 0x41

uint8_t joy1_valid = false ;
uint8_t joy2_valid = false ;


// 竖屏
#if PLAYER_MODE == PMODE_GB
struct OuputGPIO _ouput_pins[] = {
	{16 ,1} , // -> 4(start)
	{23 ,0} , // -> 34(up)
	{17 ,1} , // -> 25(A)
	{18 ,1} , // -> 27(B)
} ;
struct InputGPIO _input_pins[] = {
	{ JOYPAD1_BTN_UP, 34, 1 } ,
	{ JOYPAD1_BTN_DOWN, 39, 0 } ,
	{ JOYPAD1_BTN_LEFT, 35, 0 } ,
	{ JOYPAD1_BTN_RIGHT, 26, 1 } ,		// 26
	{ JOYPAD1_BTN_A, 25, 0 } ,			// 25
	{ JOYPAD1_BTN_B, 27, 0 } ,
	{ JOYPAD1_BTN_X, 255, 0 } ,
	{ JOYPAD1_BTN_Y, 255, 0 } ,
	{ JOYPAD1_BTN_START, 4, 0 } ,
	{ JOYPAD1_BTN_SELECT, 255, 0 } ,
	
	{ JOYPAD2_BTN_UP, 255, 1 } ,
	{ JOYPAD2_BTN_DOWN, 255, 0 } ,
	{ JOYPAD2_BTN_LEFT, 255, 0 } ,
	{ JOYPAD2_BTN_RIGHT, 255, 1 } ,
	{ JOYPAD2_BTN_A, 255, 0 } ,
	{ JOYPAD2_BTN_B, 255, 0 } ,
	{ JOYPAD2_BTN_X, 255, 0 } ,
	{ JOYPAD2_BTN_Y, 255, 0 } ,
	{ JOYPAD2_BTN_START, 255, 0 } ,
	{ JOYPAD2_BTN_SELECT, 255, 0 } ,
} ;

#elif PLAYER_MODE == PMODE_PSP
// 横屏
struct OuputGPIO _ouput_pins[] = {
	// 屏幕电源
	{23 ,1} ,	// vcc
	{22 ,0} ,	// gnd

	{27 ,0} , // -> 18(DOWN)
	{17 ,1} , // -> 26(RIGHT)
	{13 ,0} , // -> 12(A)

} ;

struct InputGPIO _input_pins[] = {
	{ JOYPAD1_BTN_UP, 36, 1 } ,
	{ JOYPAD1_BTN_DOWN, 18, 1 } ,
	{ JOYPAD1_BTN_LEFT, 19, 0 } ,
	{ JOYPAD1_BTN_RIGHT, 26, 0 } ,
	{ JOYPAD1_BTN_A, 12, 1 } ,
	{ JOYPAD1_BTN_B, 255, 0 } ,    // 导致 usb tx 冲突
	{ JOYPAD1_BTN_START, 255, 0 } ,
	{ JOYPAD1_BTN_SELECT, 255, 0 } ,
} ;

#elif PLAYER_MODE == PMODE_FC

struct OuputGPIO _ouput_pins[] = {
} ;
struct InputGPIO _input_pins[] = {
	{ JOYPAD1_BTN_UP, 255, 1 } ,
	{ JOYPAD1_BTN_DOWN, 255, 0 } ,
	{ JOYPAD1_BTN_LEFT, 255, 0 } ,
	{ JOYPAD1_BTN_RIGHT, 255, 1 } ,
	{ JOYPAD1_BTN_A, 255, 0 } ,
	{ JOYPAD1_BTN_B, 255, 0 } ,
	{ JOYPAD1_BTN_X, 255, 0 } ,
	{ JOYPAD1_BTN_Y, 255, 0 } ,
	{ JOYPAD1_BTN_START, 255, 0 } ,
	{ JOYPAD1_BTN_SELECT, 255, 0 } ,
	{ JOYPAD2_BTN_UP, 255, 1 } ,
	{ JOYPAD2_BTN_DOWN, 255, 0 } ,
	{ JOYPAD2_BTN_LEFT, 255, 0 } ,
	{ JOYPAD2_BTN_RIGHT, 255, 1 } ,
	{ JOYPAD2_BTN_A, 255, 0 } ,
	{ JOYPAD2_BTN_B, 255, 0 } ,
	{ JOYPAD2_BTN_X, 255, 0 } ,
	{ JOYPAD2_BTN_Y, 255, 0 } ,
	{ JOYPAD2_BTN_START, 255, 0 } ,
	{ JOYPAD2_BTN_SELECT, 255, 0 } ,
} ;

#endif






esp_err_t read_joy_i2c_data(uint8_t i2cbus, uint8_t addr, uint8_t * data){
	i2c_cmd_handle_t  cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_READ, true);
	if(data!=NULL) {
		i2c_master_read(cmd, data, 5, I2C_MASTER_ACK);
		i2c_master_read(cmd, data+5, 1, I2C_MASTER_NACK);
	}
	i2c_master_stop(cmd);
	esp_err_t res = i2c_master_cmd_begin(i2cbus, cmd, 10/portTICK_PERIOD_MS) ;
	i2c_cmd_link_delete(cmd);
	return res ;
}

uint8_t joydata_buff[12] ;
void ini_joys () {

	printf("joy1 sda: %d, scl: %d \n",JOY1_SDA,JOY1_SCL) ;

	i2c_config_t i2c_config = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = JOY1_SDA,
		.scl_io_num = JOY1_SCL,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = JOY_FREQ
	};
	if(i2c_param_config(0, &i2c_config)!=ESP_OK) {
		printf("i2c_param_config() faild\n") ;
        return ;
    }
	if(i2c_driver_install(0, I2C_MODE_MASTER, 0, 0, 0)!=ESP_OK) {
		printf("i2c_driver_install() faild\n") ;
        return ;
    }

	int timeout ;
	i2c_get_timeout(0, &timeout) ;
	printf("i2c timeout %d \n", timeout) ;

	joy1_valid = read_joy_i2c_data(I2C_NUM_0, JOY1_ADDR, joydata_buff)==ESP_OK ;
	joy2_valid = read_joy_i2c_data(I2C_NUM_0, JOY2_ADDR, joydata_buff+6)==ESP_OK ;

	printf("check joy1: %d\n", joy1_valid) ;
	printf("check joy2: %d\n", joy2_valid) ;
}

void init_button() {

    gpio_config_t io_conf;

	// output pins
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	for(int i=0; i< sizeof(_ouput_pins)/sizeof(struct OuputGPIO); i++ ) {
		printf("set gpio %d as output, value: %d\n", _ouput_pins[i].pin, _ouput_pins[i].value) ;

		io_conf.pin_bit_mask = 1ULL << _ouput_pins[i].pin;
		gpio_config(&io_conf);
		gpio_set_level(_ouput_pins[i].pin, _ouput_pins[i].value);
	}

	// input pins
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
	for(int i=0; i< sizeof(_input_pins)/sizeof(struct InputGPIO); i++ ) {
		if(_input_pins[i].pin == 255) {
			continue ;
		}
		printf("set gpio %d as intput, pull: %d\n", _input_pins[i].pin, _input_pins[i].pull) ;
		io_conf.pin_bit_mask = 1ULL << _input_pins[i].pin ;
		// 上拉输入
		if(_input_pins[i].pull) {
    		io_conf.pull_up_en = 1;
    		io_conf.pull_down_en = 0;
		}
		// 下拉输入
		else {
    		io_conf.pull_up_en = 0;
    		io_conf.pull_down_en = 1;
		}
		gpio_config(&io_conf);
	}
}



void print_input(uint8_t btnbase) {
	printf("[joy%d] up=%d,down=%d,left=%d,right=%d,A=%d,B=%d,st=%d,sel=%d\n", 
			btnbase?2:1 ,
			_input_pins[btnbase+JOYPAD1_BTN_UP].pressdown ,
			_input_pins[btnbase+JOYPAD1_BTN_DOWN].pressdown ,
			_input_pins[btnbase+JOYPAD1_BTN_LEFT].pressdown ,
			_input_pins[btnbase+JOYPAD1_BTN_RIGHT].pressdown ,
			_input_pins[btnbase+JOYPAD1_BTN_A].pressdown ,
			_input_pins[btnbase+JOYPAD1_BTN_B].pressdown ,
			_input_pins[btnbase+JOYPAD1_BTN_START].pressdown ,
			_input_pins[btnbase+JOYPAD1_BTN_SELECT].pressdown
	) ;
}


const uint8_t BTN_CNT = sizeof(_input_pins)/sizeof(struct InputGPIO) ;

void init_input() {
	#if PLAYER_MODE == PMODE_GB
		init_button() ;
	#endif

	#if PLAYER_MODE == PMODE_FC
		ini_joys() ;
	#endif
}

void set_btn_press(uint8_t btn, uint8_t pressdown) {
	pressdown = pressdown? 1: 0 ;
	_input_pins[btn].changed = (_input_pins[btn].pressdown != pressdown)? 1: 0 ;
	_input_pins[btn].pressdown = pressdown ;
}

void read_joy(uint8_t i2cbus, uint8_t addr, uint8_t btnbase) {

	uint8_t * joydata = joydata_buff ;
	if(btnbase>0){
		joydata+= 6 ;
	}
	
	esp_err_t res = read_joy_i2c_data(I2C_NUM_0, addr, joydata) ;
	if(res!=ESP_OK) {
		printf("i2c_master_cmd_begin() not ok, res=%d, joy:%d \n", res, btnbase) ;
	}
	else {

		// uint16_t h = (joydata[1]<<8) | joydata[0] ;
		// uint16_t v = (joydata[3]<<8) | joydata[2] ;

		// set_btn_press(btnbase+JOYPAD1_BTN_UP, v>700) ;
		// set_btn_press(btnbase+JOYPAD1_BTN_DOWN, v<300) ;
		// set_btn_press(btnbase+JOYPAD1_BTN_LEFT, h>700) ;
		// set_btn_press(btnbase+JOYPAD1_BTN_RIGHT, h<300) ;

		set_btn_press(btnbase+JOYPAD1_BTN_A, joydata[4]&0x1) ;
		set_btn_press(btnbase+JOYPAD1_BTN_B, joydata[4]&0x2) ;
		// set_btn_press(btnbase+JOYPAD1_BTN_X, joydata[4]&0x4) ;
		// set_btn_press(btnbase+JOYPAD1_BTN_Y, joydata[4]&0x8) ;
		set_btn_press(btnbase+JOYPAD1_BTN_START, joydata[4]&0x10) ;
		set_btn_press(btnbase+JOYPAD1_BTN_SELECT, joydata[4]&0x20) ;

		set_btn_press(btnbase+JOYPAD1_BTN_UP, joydata[5]&0x1) ;
		set_btn_press(btnbase+JOYPAD1_BTN_RIGHT, joydata[5]&0x2) ;
		set_btn_press(btnbase+JOYPAD1_BTN_DOWN, joydata[5]&0x4) ;
		set_btn_press(btnbase+JOYPAD1_BTN_LEFT, joydata[5]&0x8) ;

		
		// printf("%d,%d,%d,%d,%d,%d\n", joydata[0], joydata[1], joydata[2], joydata[3], joydata[4], joydata[5]) ;

		// printf("up press=%d, changed=%d\n",_input_pins[JOYPAD1_BTN_UP].changed,_input_pins[JOYPAD1_BTN_UP].pressdown) ;
	}
}

void read_pin() {
	uint8_t value, pressdown ;
	for(int i=0; i<BTN_CNT; i++ ) {
		if(_input_pins[i].pin==255)
			continue ;
		set_btn_press( i, gpio_get_level(_input_pins[i].pin) != _input_pins[i].pull ) ;
	}
	
	// 缺少 select 定义， 用 left/right 组合键
	if(_input_pins[JOYPAD1_BTN_SELECT].pin==255) {
		set_btn_press(JOYPAD1_BTN_SELECT, _input_pins[JOYPAD1_BTN_LEFT].pressdown && _input_pins[JOYPAD1_BTN_RIGHT].pressdown) ;
	}
	
	// 缺少 start 定义
	if(_input_pins[JOYPAD1_BTN_START].pin==255) {

		// 用 up + down = select
		set_btn_press(JOYPAD1_BTN_START, _input_pins[JOYPAD1_BTN_UP].pressdown && _input_pins[JOYPAD1_BTN_DOWN].pressdown) ;
	}
}

struct InputGPIO * loop_input() {

	#if PLAYER_MODE == PMODE_GB
		read_pin() ;
	#elif PLAYER_MODE == PMODE_FC
		read_joy(I2C_NUM_0, JOY1_ADDR, JOYPAD1_BTNS) ;
		if(joy2_valid) {
			read_joy(I2C_NUM_0, JOY2_ADDR, JOYPAD2_BTNS) ;
		}
	#endif
	
	// select + start = restart
	if(  (_input_pins[JOYPAD1_BTN_SELECT].pressdown && _input_pins[JOYPAD1_BTN_START].pressdown) ){
		printf("[joy1] select + start = restart; raw: %d,%d,%d,%d,%d,%d\n", joydata_buff[0], joydata_buff[1], joydata_buff[2], joydata_buff[3], joydata_buff[4], joydata_buff[5]) ;
		esp_restart() ;
	}
	// select + start = restart
	if( (_input_pins[JOYPAD2_BTN_SELECT].pressdown && _input_pins[JOYPAD2_BTN_START].pressdown) ){
		printf("[joy2] select + start = restart; raw: %d,%d,%d,%d,%d,%d\n", joydata_buff[6], joydata_buff[7], joydata_buff[8], joydata_buff[9], joydata_buff[10], joydata_buff[11]) ;
		esp_restart() ;
	}

	return _input_pins ;
}

void test_input() {
	for(int i=0; i<JOYPAD2_BTNS_END; i++) {
		if(_input_pins[i].changed) {
			print_input(JOYPAD1_BTNS) ;
			print_input(JOYPAD2_BTNS) ;
			break ;
		}
	}
}

struct InputGPIO * input_pins() {
    return _input_pins ;
}

uint8_t btn_has_click(uint8_t btn) {
	return (_input_pins[ btn ].changed && !_input_pins[ btn ].pressdown)? 1: 0 ;
}
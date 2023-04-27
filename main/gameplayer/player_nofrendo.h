#ifndef ONCE_H_PLAYER_NOFRENDO
#define ONCE_H_PLAYER_NOFRENDO

#include <stdint.h>
#include <stdbool.h>
#include "disp_st77XX.h"

#define  VIDEO_WIDTH        256
#define  VIDEO_HEIGHT       240

void player_nofrendo_set_video(st77xx_dev_t * dev, uint16_t * buff, uint16_t width, uint16_t lines, uint16_t xr, uint16_t xw) ;
int player_nofrendo_load_rom(char * rompath) ;

int player_nofrendo_main(uint8_t showcase) ;

#endif
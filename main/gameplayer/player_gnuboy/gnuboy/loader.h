#ifndef __LOADER_H__
#define __LOADER_H__


typedef struct loader_s
{
	char *rom;
	char *base;
	char *sram;
	char *state;
	int ramloaded;
} loader_t;


extern loader_t loader;

void gnuboy_loader_init(char * romPath);
void gnuboy_loader_unload();
int gnuboy_rom_load(char *romPath);
int gnuboy_sram_load();
int gnuboy_sram_save();
void gnuboy_state_load(int n);
void gnuboy_state_save(int n);



#endif

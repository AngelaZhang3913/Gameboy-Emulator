#include "typedef.h"
#include <SDL.h>
#include <glad/glad.h>
#include <fstream>

extern BYTE screen_data[144*4][160*4][3] ;
extern void update_graphics(int cycles) ;
extern void render_screen();
extern void resize_window();

extern int WIDTH;
extern int HEIGHT;

extern void display() ;

extern void create_window();

extern SDL_Window* window;

extern bool is_lcd_enabled();

extern std::ofstream myfile;
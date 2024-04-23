#include "typedef.h"
#include <SDL.h>
#include <glad/glad.h>


extern BYTE screen_data[160][144][3] ;
extern void update_graphics(int cycles) ;
extern void render_screen();

extern int WIDTH;
extern int HEIGHT;

extern void display() ;

extern void create_window();

extern SDL_Window* window;
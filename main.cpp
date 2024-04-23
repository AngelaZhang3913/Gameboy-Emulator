// On linux compile with:
// g++ -std=c++17 main.cpp glad/src/glad.c -I./glad/include -o prog -lSDL2 -ldl
// On windows compile with (if using mingw)
// g++ main.cpp ./glad/src/glad.c -I./glad/include -o prog.exe -lmingw32 -lSDL2main -lSDL2

#include <iostream>
#include "hardware.h"
#include "load_game.h"
#include "emulator.h"
#include "graphics.h"
#include "joypad.h"
#include<unistd.h>

static BYTE bios [0x100] = {
	0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
	0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
	0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
	0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
	0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
	0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
	0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
	0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
	0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xF2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
	0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
	0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
	0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
	0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
	0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3c, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x4C,
	0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
	0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50 };

int main(int argc, char* argv[]){

    load_game_method();

    create_window();

    // Infinite loop for our application
    bool gameIsRunning = true;

    bool started = false;
    while(gameIsRunning){
        if(!started) {
            memcpy(cartridge_memory, bios, 0x100 * sizeof(BYTE));
            printf("%x\n", cartridge_memory[0x101]);
        }
        update();
        started = true;

        glViewport(0,0,640,480);

        SDL_Event event;
        // Start our event loop
        while(SDL_PollEvent(&event)){
            // Handle each specific event
            if(event.type == SDL_QUIT){
                gameIsRunning= false;
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT:
                        key_pressed(0);
                        break;
                    case SDLK_LEFT:
                        key_pressed(1);
                        break;
                    case SDLK_UP:
                        key_pressed(2);
                        break;
                    case SDLK_DOWN:
                        key_pressed(3);
                        break;
                    case SDLK_a:
                        key_pressed(4);
                        break;
                    case SDLK_s:
                        key_pressed(5);
                        break;
                    case SDLK_SPACE:
                        key_pressed(6);
                        break;
                    case SDLK_RETURN:
                        key_pressed(7);
                        break;
                }
            }
        }

        /*glClearColor(1.0f,0.0f,0.0f,1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);*/

        SDL_GL_SwapWindow(window);

        //sleep(1);
        usleep(16000);
    }

    // We destroy our window. We are passing in the pointer
    // that points to the memory allocated by the 
    // 'SDL_CreateWindow' function. Remember, this is
    // a 'C-style' API, we don't have destructors.
    SDL_DestroyWindow(window);
    
    // We safely uninitialize SDL2, that is, we are
    // taking down the subsystems here before we exit
    // We add a delay in order to see that our window
    // has successfully popped up.
    SDL_Delay(3000);
    // We add a delay in order to see that our window
    // has successfully popped up.
    SDL_Delay(3000);
    // We add a delay in order to see that our window
    // has successfully popped up.
    SDL_Delay(3000);
    // We add a delay in order to see that our window
    // has successfully popped up.
    SDL_Delay(3000);
    // We add a delay in order to see that our window
    // has successfully popped up.
    SDL_Delay(3000);
    // our program.
    SDL_Quit();
    return 0;
}

/*#include "hardware.h"
#include "load_game.h"
#include "emulator.h"
#include<unistd.h>
#include <SDL.h>
#include <iostream>
#include "graphics.h"

int main(int argc, char* argv[]) {
    printf("hello");
    load_game_method(); // possibly use args to determine which game to load

    // set up glut window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(WIDTH, HEIGHT); 
    glutInitWindowPosition(100, 100); 

    int MainWindow = glutCreateWindow("Hello Graphics!!"); 
    glClearColor(0.0, 0.0, 0.0, 0);

    while (true) { // call the update method 60 times in a second
        update();
        usleep(16000);
    }
}*/

/*
            while(SDL_PollEvent(&event)){
                // Handle each specific event
                if(event.type == SDL_QUIT){
                    gameIsRunning= false;
                }

                if(event.type == SDL_MOUSEMOTION){
                    std::cout << "mouse has been moved\n";
                }
                if(event.type == SDL_KEYDOWN){
                    std::cout << "a key has been pressed\n";
                    if(event.key.keysym.sym == SDLK_0){
                        std::cout << "0 was pressed\n";
                    }else{
                        std::cout << "0 was not pressed\n";
                    }
                }
                // Retrieve the state of all of the keys
                // Then we can query the scan code of one or more
                // keys at a time
                const Uint8* state = SDL_GetKeyboardState(NULL);
                if(state[SDL_SCANCODE_RIGHT]){
                    std::cout << "right arrow key is pressed\n";
                }
            }*/
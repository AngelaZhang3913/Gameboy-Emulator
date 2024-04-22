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

int main(int argc, char* argv[]){

    load_game_method();

    create_window();

    // Infinite loop for our application
    bool gameIsRunning = true;
    while(gameIsRunning){
        update();

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

        sleep(2);
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
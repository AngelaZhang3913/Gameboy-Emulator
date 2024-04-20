#include "hardware.h"
#include "load_game.h"
#include "emulator.h"
#include<unistd.h>
#include <glut/glut.h>
#include "graphics.h"

int main(int argc, char* argv[]) {
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
        sleep(0.016);
    }
}



/*void makePixel(int x, int y, int r, int g, int b, GLubyte* pixels, int width, int height)
{
    if (0 <= x && x < width && 0 <= y && y < height) {
        int position = (x + y * width) * 3;
        pixels[position] = r;
        pixels[position + 1] = g;
        pixels[position + 2] = b;
    }
}*/

/*int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(WIDTH, HEIGHT); 
    glutInitWindowPosition(100, 100); 

    int MainWindow = glutCreateWindow("Hello Graphics!!"); 
    glClearColor(0.0, 0.0, 0.0, 0);

    //makePixel(200,200,255,255,255,PixelBuffer, WIDTH, HEIGHT);
    glutDisplayFunc(display); 
    glutMainLoop();
    return 0;
}*/
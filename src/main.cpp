#include <iostream>
#include "SimpleWindow.h"

/*
 * SDL may 'hijack' main. Including SDL.h code directly inside main, will cause linker errors.
 * To circumvent this, you can
 *  1) Not include SDL inside main, and put them in another class (i.e SimpleWindow)
 *  2) Add '#define SDL_MAIN_HANDLED' before including SDL
 */

int main(int argc, char *argv[]) {
    SimpleWindow window;
    window.run();
    return 0;
}
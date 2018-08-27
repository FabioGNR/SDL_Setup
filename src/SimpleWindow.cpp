//
// Created by Zain on 27/08/2018.
//

#include <SDL.h>
#include "SimpleWindow.h"

void SimpleWindow::run() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
            "An SDL2 window",         //    const char* title
            SDL_WINDOWPOS_UNDEFINED,  //    int x: initial x position
            SDL_WINDOWPOS_UNDEFINED,  //    int y: initial y position
            640,                      //    int w: width, in pixels
            480,                      //    int h: height, in pixels
            SDL_WINDOW_SHOWN          //    Uint32 flags: window options, see docs
    );

    if (window == nullptr) {
        SDL_Quit();
    }
    SDL_Delay(3000);  // Wait for 3000 milliseconds, for example

    SDL_DestroyWindow(window);
    SDL_Quit();
}

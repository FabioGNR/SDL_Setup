//
// Created by Zain on 27/08/2018.
//

#include "SimpleWindow.h"
#include <SDL.h>

void SimpleWindow::run() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "SDL Setup",            // Window title
        SDL_WINDOWPOS_CENTERED, // X pos
        SDL_WINDOWPOS_CENTERED, // Y Pos
        640, 480,               // Width, Height
        SDL_WINDOW_SHOWN        // Window Flag
    );

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;

    bool run = true;
    while (run) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run = false;
            }
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

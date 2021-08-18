#include <SDL.h>

#include "types.h"
#include "globals.h"

static SDL_Window* window;
static SDL_Renderer* renderer;

void init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(windowWidth, windowHeight, SDL_WINDOW_OPENGL, &window, &renderer);
}

void quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main()
{
    init();
    int run = 1;
    while (run) {
        int ticks = SDL_GetTicks();

        // input
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) {
                run = 0;
            }
        }

        // update

        // render
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        int fps = 30;
        SDL_Delay(ticks + 1000 / fps - SDL_GetTicks());
    }

    quit();
}
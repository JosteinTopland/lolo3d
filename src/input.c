#include "input.h"

#include <SDL2/SDL.h>
#include "globals.h"

void input_keys() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            SDL_Keycode sym = event.key.keysym.sym;
            if (sym == SDLK_LEFT) camera_x += 0.1f;
            if (sym == SDLK_RIGHT) camera_x -= 0.1f;
            if (sym == SDLK_UP) camera_y += 0.1f;
            if (sym == SDLK_DOWN) camera_y -= 0.1f;
            if (sym == SDLK_w) {
                GLint polyMode[2];
                glGetIntegerv(GL_POLYGON_MODE, polyMode);
                glPolygonMode(GL_FRONT_AND_BACK, polyMode[0] == GL_FILL ? GL_LINE : GL_FILL);
            }
            if (sym == SDLK_q) {
                run = 0;
            }
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
            run = 0;
        }
    }
}

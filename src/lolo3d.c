#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "globals.h"
#include "shader_loader.h"
#include "level.h"
#include "render.h"
#include "input.h"

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Lolo 3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    glewInit();
    if (!installShaders()) {
        fprintf(stderr, "Error loading shaders.\n");
        return 1;
    }
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, windowWidth, windowHeight);

    loadLevel();
    while (run) {
        int ticks = SDL_GetTicks();

        input_keys();
        render_level(model);
        SDL_GL_SwapWindow(window);

        // wait
        int fps = 100;
        int delay = 1000 / fps - (SDL_GetTicks() - ticks);
        if (delay > 0) SDL_Delay(delay);
    }
    //freeModel(model);

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
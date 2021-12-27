#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <GL/glew.h>

#include "globals.h"
#include "shader_loader.h"
#include "level.h"
#include "render.h"
#include "input.h"

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    window = SDL_CreateWindow("Lolo 3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024);
    Mix_Music *music = Mix_LoadMUS("assets/christmas_dance_mix.mod");
    Mix_VolumeMusic(10);
    Mix_PlayMusic(music, -1);

    glewInit();

    const unsigned char *glVersion = glGetString(GL_VERSION);
    printf("GL_VERSION: %s\n", glVersion);

    if (!installShaders()) return 1;
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, windowWidth, windowHeight);

    loadLevel();
    while (run) {
        int ticks = SDL_GetTicks();

        input_keys();
        render_level(level);
        SDL_GL_SwapWindow(window);

        // wait
        int fps = 60;
        int delay = 1000 / fps - (SDL_GetTicks() - ticks);
        if (delay > 0) SDL_Delay(delay);
    }
    //freeModel(model);

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(music);
    //Mix_CloseAudio();
    //SDL_Quit();

    return 0;
}
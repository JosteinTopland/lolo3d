#include <SDL2/SDL.h>

#include "globals.h"
#include "input.h"
#include "level.h"
#include "render.h"
#include "shader_loader.h"

void init()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    int width = 800;
    int height = 600;
    SDL_Window *window = SDL_CreateWindow("Lolo 3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(window);

    glewInit();
    install_shaders();

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);
    glm_perspective(glm_rad(50.0f), (float) width / height, 0.1f, 200.0f, proj_mat);
    glUniformMatrix4fv(proj_mat_id, 1, GL_FALSE, &proj_mat[0][0]);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024);
}

void deinit()
{
    Mix_CloseAudio();

    SDL_GLContext glContext = SDL_GL_GetCurrentContext();
    SDL_GL_DeleteContext(glContext);

    SDL_Window* window = SDL_GL_GetCurrentWindow();
    SDL_DestroyWindow(window);

    SDL_Quit();    
}

int main(int argc, char* argv[])
{
    init();
 
    SDL_Window *window = SDL_GL_GetCurrentWindow();
    Level *level = load_level(0);
    while (run) {
        int ticks = SDL_GetTicks();

        input_keys();
        render_level(level);
        SDL_GL_SwapWindow(window);

        int fps = 60;
        int delay = 1000 / fps - (SDL_GetTicks() - ticks);
        if (delay > 0) SDL_Delay(delay);
    }
    free_level(level);

    deinit();
    return 0;
}

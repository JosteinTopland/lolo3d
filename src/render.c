#include "render.h"

#include <SDL2/SDL.h>
#include "globals.h"

void render(Model *model) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, 1); // TODO textureid
    //glUniform1i(samplerUniformLocation, 0); // GL_TEXTURE0
    glUniform1i(enableTexture, true);

    glMultiDrawArrays(GL_TRIANGLES, model->faceFirst, model->faceCount, model->numFaces);
}
#ifndef GLHEADER_H
#define GLHEADER_H

#ifdef WINDOWS
#include <GL/glew.h>
#elif RPI
#include <SDL2/SDL_opengles2.h>
#endif
#endif

#pragma once

#define NO_SDL_GLEXT
#include <SDL_opengl.h>
#include <SDL.h>
#include <SDL_thread.h>
#include <SDL_main.h>

// Enable OpenGL 2.0 shader functions. Return 0 on error.
int enableShaderProcs(void);

#if (defined __APPLE__)
  #include <OpenGL/glu.h>
  #include <OpenGL/glext.h>
  int enableShaderProcs(void) { return 1; }
#elif (defined __WIN32__)
  #define GL_IMPORT_NEEDED
#elif (defined __linux__)
  #include <GL/glx.h>
  #include <GL/glxext.h>
  #define GL_IMPORT_NEEDED
#else
  int enableShaderProcs(void) { return 0; }
#endif

#include "GlShaderFunctions.h"

void CreateCursors();

extern SDL_Cursor* arrow_cursor;
extern SDL_Cursor* hand_cursor;


//EOF

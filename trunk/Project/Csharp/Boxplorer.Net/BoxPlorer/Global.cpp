#include "Precompiled.h"

using namespace std;

int program;

// Our SDL handle.
SDL_Surface* screen;

// Optional #defines for glsl compilation from .cfg file.
string defines;

StereoMode stereoMode = ST_NONE;

// ogl framebuffer object
GLuint fbo;
// texture that frame got rendered to
GLuint texture;
// depth buffer attached to fbo
GLuint depthBuffer;
// the dof mipmapper program
int dof_program;
// texture holding background image
GLuint background_texture;

// Position of the OpenGL window on the screen.
int viewportOffset[2];

// Is the mouse and keyboard input grabbed?
int grabbedInput = 1;

char* parName[NUMPARS][3];
KeyFrame camera; // Active camera view
KeyFrame config;  // Global configuration set
vector<KeyFrame> keyframes;  // Keyframes

int framesToAverage;
Uint32* frameDurations;
int frameDurationsIndex = 0;
Uint32 lastFrameTime;

const char* kKEYFRAME = "keyframe";


//EOF

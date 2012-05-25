#pragma once

#include <vector>
#include <string>
#include "KeyFrame.h"
#include "Sdl.h"

// Pinhole camera modes.
enum StereoMode 
{ 
	ST_NONE=0, 
	ST_OVERUNDER, 
	ST_XEYED, 
	ST_INTERLACED 
};

extern int program;
extern KeyFrame config;

// Our SDL handle.
extern SDL_Surface* screen;
// Optional #defines for glsl compilation from .cfg file.
extern std::string defines;
extern StereoMode stereoMode;
// ogl framebuffer object
extern GLuint fbo;
// texture that frame got rendered to
extern GLuint texture;
// depth buffer attached to fbo
extern GLuint depthBuffer;
// the dof mipmapper program
extern int dof_program;
// texture holding background image
extern GLuint background_texture;
// Position of the OpenGL window on the screen.
extern int viewportOffset[2];
// Is the mouse and keyboard input grabbed?
extern int grabbedInput;
extern char* parName[NUMPARS][3];
extern KeyFrame camera; // Active camera view
extern KeyFrame config;  // Global configuration set
extern std::vector<KeyFrame> keyframes;  // Keyframes
extern int framesToAverage;
extern Uint32* frameDurations;
extern int frameDurationsIndex;
extern Uint32 lastFrameTime;
extern const char* kKEYFRAME;

void suggestDeltaTime(KeyFrame& camera, const std::vector<KeyFrame>& keyframes); 

#define DEFAULT_CONFIG_FILE  "boxplorer.cfg"
#define VERTEX_SHADER_FILE   "vertex.glsl"
#define FRAGMENT_SHADER_FILE "fragment.glsl"
#define FRAME_VERTEX_SHADER_FILE   "frame_vertex.glsl"
#define FRAME_FRAGMENT_SHADER_FILE "frame_fragment.glsl"

#define FPS_FRAMES_TO_AVERAGE 20



//EOF

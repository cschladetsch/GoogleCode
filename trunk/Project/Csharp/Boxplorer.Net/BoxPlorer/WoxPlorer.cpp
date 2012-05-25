#include "Precompiled.h"
#include "Utility.h"
#include "Tga.h"
#include "DefaultShaders.h"
#include "WoxPlorer.h"

using namespace std;

#define zNear 0.0001f
#define zFar  5.0f

void suggestDeltaTime(KeyFrame& camera, const vector<KeyFrame>& keyframes) 
{
	if (keyframes.empty()) 
	{
		camera.delta_time = 0;
	} 
	else 
	{
		float dist = camera.distanceTo(keyframes[keyframes.size() - 1]);
		float steps = dist / camera.speed;
		camera.delta_time = steps / config.fps;
	}
}

void saveScreenshot(char const* tgaFile) 
{
	TGA tga;
	tga.readFramebuffer(config.width, config.height, viewportOffset);
	if (tga.writeFile(tgaFile))
		printf(__FUNCTION__ " : wrote %s\n", tgaFile);
	else
		printf(__FUNCTION__ " : failed to write %s\n", tgaFile);
}

TGA background;

void LoadBackground() 
{
	background.readFile("background.tga");
	if (background.data()) 
	{
		printf(__FUNCTION__ " : loaded background image from '%s'\n", "background.tga");
	}
}

// Return BGR value of pixel x,y.
unsigned int getBGRpixel(int x, int y) 
{
	unsigned char img[3];
	int height = config.height;
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadBuffer(GL_FRONT);
	glReadPixels(viewportOffset[0] + x, viewportOffset[1] + height - 1 - y, 1, 1, GL_BGR, GL_UNSIGNED_BYTE, img);
	unsigned int val =
		img[0] * 256 * 256 +
		img[1] * 256 +
		img[2];
	return val;
}

// Read out Z-buffer around the center of the view.
float distanceToSurface() 
{
#ifdef TRY_DISTANCE_TO_SURFACE
	const int SIZE = 1;
	float z[SIZE*SIZE];
	int x = config.width / 2 - SIZE / 2;
	int y = config.height / 2 - SIZE / 2;
	glReadPixels(viewportOffset[0] + x, viewportOffset[1] + config.height - 1 - y, SIZE, SIZE,
		GL_DEPTH_COMPONENT, GL_FLOAT, z);
	float avg = 0;
	for (size_t i = 0; i < lengthof(z); ++i) {
		avg += z[i];
	}
	float v = avg / lengthof(z);

	// Convert zbuffer [0,1] value v back into actual dist.
	const float a = zFar / (zFar - zNear);
	const float b = zFar * zNear / (zNear - zFar);

	return b / (v - a);
#else
	return 100.0f;
#endif
}

string glsl_source;

// Compile and activate shader programs. Return the program handle.
int setupShaders(void) 
{
	char const* vs;
	char const* fs;
	GLuint v,f,p;
	char log[2048]; int logLength;

	(vs = readFile(VERTEX_SHADER_FILE)) || ( vs = default_vs );
	(fs = readFile(FRAGMENT_SHADER_FILE)) || ( fs = default_fs );

	if (fs != default_fs) {
		printf(__FUNCTION__ " : read shader from %s\n", FRAGMENT_SHADER_FILE);
	} else {
		printf(__FUNCTION__ " : using default shader\n");
	}
	if (vs != default_vs) {
		printf(__FUNCTION__ " : read vertex shader from %s\n", VERTEX_SHADER_FILE);
	} else {
		printf(__FUNCTION__ " : using default vertex shader\n");
	}

	glsl_source.assign(defines + fs);

	p = glCreateProgram();

	v = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(v, 1, &vs, 0);
	glCompileShader(v);
	glGetShaderInfoLog(v, sizeof(log), &logLength, log);
	if (logLength) fprintf(stderr, __FUNCTION__ " : %s\n", log);

	f = glCreateShader(GL_FRAGMENT_SHADER);
	if (!defines.empty()) {
		const char* srcs[2] = {defines.c_str(), fs};
		glShaderSource(f, 2, srcs, 0);
	} else {
		glShaderSource(f, 1, &fs, 0);
	}
	glCompileShader(f);
	glGetShaderInfoLog(f, sizeof(log), &logLength, log);
	if (logLength) fprintf(stderr, __FUNCTION__ " : %s\n", log);

	glAttachShader(p, v);
	glAttachShader(p, f);
	glLinkProgram(p);

	glGetProgramInfoLog(p, sizeof(log), &logLength, log);
	if (logLength) fprintf(stderr, __FUNCTION__ " : %s\n", log);

	if (vs != default_vs) free((char*)vs);
	if (fs != default_fs) free((char*)fs);

	if (glGetError()) die("setupShaders() fails");

	return p>0?p:0;
}

// Compile and activate shader programs for frame buffer manipulation.
// Return the program handle.
int setupShaders2(void) 
{
	char const* vs;
	char const* fs;
	GLuint v,f,p;
	char log[2048]; int logLength;

	(vs = readFile(FRAME_VERTEX_SHADER_FILE)) || ( vs = frame_default_vs );
	(fs = readFile(FRAME_FRAGMENT_SHADER_FILE)) || ( fs = frame_default_fs );

	if (fs != frame_default_fs) {
		printf(__FUNCTION__ " : read shader from %s\n", FRAME_FRAGMENT_SHADER_FILE);
	} else {
		printf(__FUNCTION__ " : using default shader\n");
	}

	glsl_source.append(fs);

	p = glCreateProgram();

	v = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(v, 1, &vs, 0);
	glCompileShader(v);
	glGetShaderInfoLog(v, sizeof(log), &logLength, log);
	if (logLength) fprintf(stderr, __FUNCTION__ " : %s\n", log);

	f = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(f, 1, &fs, 0);
	glCompileShader(f);
	glGetShaderInfoLog(f, sizeof(log), &logLength, log);
	if (logLength) fprintf(stderr, __FUNCTION__ " : %s\n", log);

	glAttachShader(p, v);
	glAttachShader(p, f);
	glLinkProgram(p);

	glGetProgramInfoLog(p, sizeof(log), &logLength, log);
	if (logLength) fprintf(stderr, __FUNCTION__ " : %s\n", log);

	if (vs != frame_default_vs) free((char*)vs);
	if (fs != frame_default_fs) free((char*)fs);

	if (glGetError()) die("setupShaders() fails");

	return p>0?p:0;
}

// Detach & delete any shaders, delete program.
void cleanupShaders(int p) 
{
	GLuint shaders[2];
	GLsizei count = 2;
	if (!glIsProgram(p)) return;
	glGetAttachedShaders(p, count, &count, shaders);
	for (GLsizei i = 0; i < count; ++i) {
		glDetachShader(p, shaders[i]);
		glDeleteShader(shaders[i]);
	}
	glDeleteProgram(p);
}

// Initializes the video mode, OpenGL state, shaders, camera and shader parameters.
// Exits the program if an error occurs.
void initGraphics() 
{
	GLenum status = GL_NO_ERROR;

	// If not fullscreen, use the color depth of the current video mode.
	int bpp = 24;  // FSAA works reliably only in 24bit modes
	if (!config.fullscreen) 
	{
		SDL_VideoInfo const* info = SDL_GetVideoInfo();
		bpp = info->vfmt->BitsPerPixel;
	}

	// Set attributes for the OpenGL window.
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, config.depth_size);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	if (config.multisamples == 1) 
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
	} 
	else 
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, config.multisamples);
	}

	// Set the video mode, hide the mouse and grab keyboard and mouse input.
	if (screen == NULL) 
		SDL_putenv((char*)"SDL_VIDEO_CENTERED=center");
	if (screen != NULL) 
		SDL_FreeSurface(screen);
	
	(screen = SDL_SetVideoMode(config.width, config.height, bpp,
		SDL_OPENGL | (config.fullscreen ? SDL_FULLSCREEN : SDL_RESIZABLE)))
		|| die("Video mode initialization failed: %s\n", SDL_GetError());

	if (config.multisamples > 1) {
		glEnable(GL_MULTISAMPLE);  // redundant?
		int i;
		SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &i);
		printf(__FUNCTION__ " : multi says %d, asked for %d\n",
			i, config.multisamples);
	}

	SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &config.depth_size);
	printf(__FUNCTION__ " : depth size %u\n", config.depth_size);
	
	//// CJS TODO tmp removal
	//if (grabbedInput) {
	//	// order is important
	//	SDL_ShowCursor(SDL_DISABLE);
	//	SDL_WM_GrabInput(SDL_GRAB_ON);
	//} else {
	//	SDL_ShowCursor(SDL_ENABLE);
	//	SDL_WM_GrabInput(SDL_GRAB_OFF);
	//}

	// TODO: logging.
	//int samples = 0; glGetIntegerv(GL_SAMPLES, &samples); printf("%dx%d, %d bpp, FSAA %d\n", screen->w, screen->h, screen->format->BitsPerPixel, samples);

	// If we got higher resolution (which can happen in fullscreen mode),
	// use a centered viewport.
	viewportOffset[0] = (screen->w - config.width)/2;
	viewportOffset[1] = (screen->h - config.height)/2;
	glViewport(viewportOffset[0], viewportOffset[1], config.width, config.height);

	//CreateCursors();

	// Enable shader functions and compile shaders.
	// Needs to be done after setting the video mode.
	enableShaderProcs() || die("This program needs support for GLSL shaders.\n");

	cleanupShaders(program);
	cleanupShaders(dof_program);

	// CJS: oh FFS, Perl coders trying C++ is rofl
	(program = setupShaders()) || die("Error in GLSL shader compilation (see stderr.txt for details).\n");

	if (background_texture == 0 && background.data() != NULL) 
	{
		// Load background image into texture
		glGenTextures(1, &background_texture);
		glBindTexture(GL_TEXTURE_2D, background_texture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, background.width(), background.height(),
			0, GL_BGR, GL_UNSIGNED_BYTE, background.data());
		glGenerateMipmap(GL_TEXTURE_2D);
		printf(__FUNCTION__ " : background texture at %d\n", background_texture);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if ((status = glGetError()) != GL_NO_ERROR)
		die(__FUNCTION__ "[%d] : glGetError() : %04x\n", __LINE__, status);

	if (!config.enable_dof) return;

	if ((status = glGetError()) != GL_NO_ERROR)
		die(__FUNCTION__ "[%d] : glGetError() : %04x\n", __LINE__, status);

	(dof_program = setupShaders2()) ||
		die("Error in GLSL shader compilation (see stderr.txt for details).\n");


	// Create depthbuffer
	glDeleteRenderbuffers(1, &depthBuffer);
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		config.width, config.height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if ((status = glGetError()) != GL_NO_ERROR)
		die(__FUNCTION__ "[%d] : glGetError() : %04x\n", __LINE__, status);

	// Create texture to render to
	glDeleteTextures(1, &texture);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Allocate storage
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, config.width, config.height,
		0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Allocate / generate mips
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Create framebuffer
	glDeleteFramebuffers(1, &fbo);
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Attach texture to framebuffer as colorbuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, texture, 0);

	// Attach depthbuffer to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuffer);

	if ((status = glGetError()) != GL_NO_ERROR)
		die(__FUNCTION__ "[%d] : glGetError() : %04x\n", __LINE__, status);

	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		die(__FUNCTION__ " : glCheckFramebufferStatus() : %04x\n", status);

	// Back to normal framebuffer.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if ((status = glGetError()) != GL_NO_ERROR)
		die(__FUNCTION__ "[%d] : glGetError() : %04x\n", __LINE__, status);
}

//EOF

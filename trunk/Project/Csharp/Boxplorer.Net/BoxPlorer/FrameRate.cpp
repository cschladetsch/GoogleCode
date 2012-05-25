#include "Precompiled.h"
#include "FrameRate.h"

float now() 
{
	return (float)SDL_GetTicks() / 1000.0f;
}

// Initialize the FPS structure.
void initFPS(int framesToAverage_) 
{
	framesToAverage = framesToAverage_;
	frameDurations = (Uint32*)malloc(sizeof(Uint32) * framesToAverage_);
	frameDurations[0] = 0;
	lastFrameTime = SDL_GetTicks();
}

// Update the FPS structure after drawing a frame.
void updateFPS(void) 
{
	Uint32 time = SDL_GetTicks();
	frameDurations[frameDurationsIndex++ % framesToAverage] = time - lastFrameTime;
	lastFrameTime = time;
}

// Return the duration of the last frame.
Uint32 getLastFrameDuration(void) 
{
	return frameDurations[(frameDurationsIndex+framesToAverage-1) % framesToAverage];
}

// Return the average FPS over the last X frames.
float getFPS(void) 
{
	if (frameDurationsIndex < framesToAverage) return 0;  // not enough data

	int i; Uint32 sum;
	for (i=sum=0; i<framesToAverage; i++) sum += frameDurations[i];
	return framesToAverage * 1000.f / sum;
}

//EOF

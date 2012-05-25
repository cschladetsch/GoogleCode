#pragma once

#include "Controller.h"

void LoadKeyFrames(bool fixedFov);
void LoadBackground();

void initGraphics();

void initFPS(int framesToAverage_);
void updateFPS(void);
float getFPS(void);

Uint32 getLastFrameDuration(void);
float getFPS(void);

unsigned int getBGRpixel(int x, int y);

void saveScreenshot(char const* tgaFile);

float now();

void cleanupShaders(int p);
void changeWorkingDirectory(const char* configFile);

float distanceToSurface();

//EOF

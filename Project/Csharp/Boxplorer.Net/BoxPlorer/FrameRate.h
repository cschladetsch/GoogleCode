#pragma once

float now();

void initFPS(int framesToAverage_);
void updateFPS(void); 
Uint32 getLastFrameDuration(void); 
float getFPS(void);

//EOF

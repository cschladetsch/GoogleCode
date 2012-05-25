#pragma once

#include <vector>
#include "KeyFrame.h"

#define NSUBFRAMES 1000  // # splined subframes between keyframes.

void CatmullRom(const std::vector<KeyFrame>& keyframes, std::vector<KeyFrame>* output, bool loop = false, int nsubframes = NSUBFRAMES);

//EOF

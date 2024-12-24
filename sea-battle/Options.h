#pragma once

#include <iostream>
#include <SDL3/SDL.h>

#include "UtilityFunctions.h"
#include "ini.h"

using namespace std;
using namespace mINI;

void CreateDefaultOptions(const string& filePath);
Uint64 GetWindowFlags(bool fullscreen, bool borderless);
bool LoadOptions(const string& filePath, SDL_Window*& window, int& width, int& height, const char* name);
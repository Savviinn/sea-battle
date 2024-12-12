#pragma once

#include <iostream>
#include <fstream>
#include <INIReader.h>
#include <SDL3/SDL.h>

using namespace std;

void CreateDefaultOptions(const string& filePath) {
	ofstream outFile(filePath);
	if (outFile) {
		outFile << "[Window]\n";
		outFile << "width=1280\n";
		outFile << "height=720\n";
		outFile << "fullscreen=false\n";
		outFile << "borderless=false\n";
		cout << "Default INI file created at: " << filePath << endl;
	}
	else {
		cerr << "Failed to create INI file at: " << filePath << endl;
	}
}

Uint64 GetWindowFlags(bool fullscreen, bool borderless) {
	Uint64 flags = 0;

	if (fullscreen) {
		flags |= SDL_WINDOW_FULLSCREEN;
	}
	if (borderless) {
		flags |= SDL_WINDOW_BORDERLESS;
	}

	return flags;
}

bool LoadOptions(const string& filePath, SDL_Window*& window, int& width, int& height, const char* name) {
	ifstream fileCheck(filePath);
	if (!fileCheck) {
		CreateDefaultOptions(filePath);
	}
	fileCheck.close();
	
	INIReader reader(filePath);

	if (reader.ParseError() == 0) {
		bool fullscreen = reader.GetBoolean("Window", "fullscreen", false);
		bool borderless = reader.GetBoolean("Window", "borderless", false);
		width  = reader.GetUnsigned("Window", "width", 1280);
		height = reader.GetUnsigned("Window", "height", 720);


		if (!SDL_Init(SDL_INIT_VIDEO)) {
			cerr << "SDL_Init error occured: " << SDL_GetError() << endl;
			return false;
		}

		if (fullscreen) {
			SDL_Rect screenSize;
			int displayCount;
			auto displays = SDL_GetDisplays(&displayCount);
			if (!displays) {
				cerr << SDL_GetError() << endl;
			}
			
			if (SDL_GetDisplayBounds(displays[0], &screenSize)) {
				if (borderless) {
					borderless = false;
				}
				width  = screenSize.w;
				height = screenSize.h;
				SDL_free(displays);
			}
			else {
				fullscreen = false;
			}
		}

		Uint64 windowFlags = GetWindowFlags(fullscreen, borderless);
		windowFlags |= !SDL_WINDOW_RESIZABLE;

		return window = SDL_CreateWindow(name, width, height, windowFlags);
	}
	cerr << "Error loading INI file" << endl;
	return false;
}
#include "Options.h"

void CreateDefaultOptions(const string& filePath) {
	INIFile file(filePath);
	INIStructure ini;

	ini["window"]["width"] = "1280";
	ini["window"]["height"] = "720";
	ini["window"]["fullscreen"] = "0 ; width, height and borderless will be ignored";
	ini["window"]["borderless"] = "0";

	file.generate(ini);

	cout << "Default INI options created at: " << filePath << endl;
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
	INIFile file(filePath);
	INIStructure ini;

	file.read(ini);
	if (!ini.has("window")) {
		CreateDefaultOptions(filePath);
	}

	string& full = ini["window"]["fullscreen"];
	string& bord = ini["window"]["borderless"];

	bool fullscreen = ExtractNumber(full, 0) == 1;
	bool borderless = ExtractNumber(bord, 0) == 1;

	string& w = ini["window"]["width"];
	width = ExtractNumber(w, 1280);
	if (width > 7680) {
		width = 7680;
	}
	else if (width < 600) {
		width = 600;
	}

	string& h = ini["window"]["height"];
	height = ExtractNumber(h, 720);
	if (height > 4320) {
		height = 4320;
	}
	else if (height < 337) {
		height = 337;
	}

	file.write(ini);


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
			width = screenSize.w;
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

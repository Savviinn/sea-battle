#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <Windows.h>
#include <iostream>
#include <string>

#include "TileList.h"
#include "TileLogic.h"

using namespace std;

int gameWindowWidth = 1280;
int gameWindowHeight = 720;
const char* gameName = "Sea battle";

SDL_Window* gameWindow;
SDL_Renderer* renderer;
SDL_Surface* tileSheetSurface;
SDL_Texture* tileSheet;

int SDL_main(int argc, char* argv[])
{
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Quit();
		return -1;
	}

	gameWindow = SDL_CreateWindow(gameName, gameWindowWidth, gameWindowHeight, !SDL_WINDOW_RESIZABLE);
	if (!gameWindow) {
		SDL_DestroyWindow(gameWindow);
		SDL_Quit();
		return -1;
	}
	renderer = SDL_CreateRenderer(gameWindow, NULL);
	if (!renderer) {
		SDL_Quit();
		return -1;
	}

	TileList tileList;
	tileList.LoadTileSheet(renderer, tileSheetSurface, 32, "map.png");

	TileLogic tileLogic;
	tileLogic.InitLogic();
	tileLogic.AttackTile(1, 1);
	tileLogic.AttackTile(1, 1);
	tileLogic.AttackTile(3, 2);
	tileLogic.AttackTile(3, 2);

	string finalString = "";
	for (auto& row : tileLogic.GetTileArray()) {
		for (auto tile : row) {
			finalString += to_string(tile) + ' ';
		}
		finalString += '\n';
	}
	//for (int x = 0; x < tileLogic.GetMapSize(); x++) {
	//	for (int y = 0; y < tileLogic.GetMapSize(); y++) {
	//		finalString += to_string(tileLogic.GetTile(x, y)) + " ";
	//	}
	//	finalString += "\n";
	//}
	cout << finalString << endl;

	Sleep(10000);
	SDL_Quit();
	return 0;
}
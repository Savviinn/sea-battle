#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <Windows.h>
#include <iostream>
#include <string>

#include "Game.h"
#include "Options.h"

using namespace std;

int gameWindowWidth;
int gameWindowHeight;
const char* gameName = "Sea battle";

string basePath    = SDL_GetBasePath();
string optionsName = "OPTIONS.ini";

SDL_Window*   gameWindow;
SDL_Renderer* renderer;
SDL_Surface*  tileSheetSurface;
SDL_Texture*  tileSheet;

int SDL_main(int argc, char* argv[])
{
	//if (!SDL_Init(SDL_INIT_VIDEO)) {
	//	SDL_Quit();
	//	return -1;
	//}

	if (!LoadOptions(basePath + optionsName, gameWindow, gameWindowWidth, gameWindowHeight, gameName)) {
		return -1;
	}

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

	Game game;
	game.InitGame(renderer, tileSheetSurface, 32, "map.png");
	game.LoadAdditionalTileSheet(renderer, tileSheetSurface, 16, "marking.png");
	auto p1 = game.GetPlayer1();
	auto p2 = game.GetPlayer2();

	p1.PlaceShip(0, 0, 3, true);
	p1.PlaceShip(3, 0, 5, true);
	p1.PlaceShip(2, 1, 4, false);
	p1.PlaceShip(-2, 0, 3, false);

	p2.PlaceShip(0, 3, 4, true);
	p2.PlaceShip(2, 3, 3, true);
	p2.PlaceShip(0, 4, 3, false);
	p2.PlaceShip(2, 3, 7, true);
	p2.PlaceShip(10, 3, 4, true);

	p1.SetTurn(true);
	p1.AttackPlayerTile(p2, 0, 5);
	p1.AttackPlayerTile(p2, 0, 6);
	p1.AttackPlayerTile(p2, 0, 7);
	p1.AttackPlayerTile(p2, 0, 8);

	p2.SetTurn(true);
	p2.AttackPlayerTile(p1, 2, 2);
	p2.AttackPlayerTile(p1, 3, 2);

	float finalCellSize = min(
		static_cast<float>(gameWindowWidth / 24.f), 
		static_cast<float>(gameWindowHeight / 12.f)
	);

	float gapSizeX = (gameWindowWidth - finalCellSize * 24.f) / 2.f + finalCellSize;
	float gapSizeY = (gameWindowHeight - finalCellSize * 12.f) / 2.f + finalCellSize;

	SDL_Event e;
	bool running = true;
	bool needsRedraw = true;
	while (running) {
		if (needsRedraw) {
			p1.Render(renderer, gapSizeX, gapSizeY, finalCellSize);
			p2.Render(renderer, gapSizeX + finalCellSize * 12.f, gapSizeY, finalCellSize);
			game.AdditionalRender(renderer, gapSizeX - finalCellSize, gapSizeY - finalCellSize, finalCellSize);
			game.AdditionalRender(renderer, gapSizeX + finalCellSize * 11.f, gapSizeY - finalCellSize, finalCellSize);

			SDL_RenderPresent(renderer);
			needsRedraw = false;
		}

		SDL_WaitEvent(&e);
		if (e.type == SDL_EVENT_QUIT) {
			running = false;
		}
		else if (e.type == SDL_EVENT_KEY_DOWN) {
			switch (e.key.scancode) {
			case SDL_SCANCODE_ESCAPE:
				running = false;
				break;
			case SDL_SCANCODE_R:
				needsRedraw = true;
				break;
			default:
				break;
			}
		}
	}

	SDL_DestroySurface(tileSheetSurface);
	SDL_DestroyTexture(tileSheet);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(gameWindow);
	IMG_Quit();
	SDL_Quit();
	return 0;
}
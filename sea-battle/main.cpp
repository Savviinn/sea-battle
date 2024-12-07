#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <Windows.h>
#include <iostream>
#include <string>

#include "Game.h"
#include "Player.h"
#include "TileList.h"
#include "test.h"

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

	Game game;
	game.InitGame(renderer, tileSheetSurface, 32, "map.png", "map.png");
	game.LoadAdditionalTileSheet(renderer, tileSheetSurface, 16, "marking.png");
	auto p1 = game.GetPlayer1();
	auto p2 = game.GetPlayer2();
	p1.SetTurn(true);p1.AttackPlayer(p2, 1, 0);
	p1.AttackPlayer(p2, 1, 1);
	p2.SetTurn(true);
	p2.AttackPlayer(p1, 2, 2);
	p2.AttackPlayer(p1, 3, 2);

	float finalCellSize = min(
		static_cast<float>(gameWindowWidth / 25.f), 
		static_cast<float>(gameWindowHeight / 12.5f)
	);

	float gapSizeX = (gameWindowWidth - finalCellSize * 25.f) / 2.f + finalCellSize;
	float gapSizeY = (gameWindowHeight - finalCellSize * 12.f) / 2.f + finalCellSize;

	p1.Render(renderer, gapSizeX, gapSizeY, finalCellSize);
	p2.Render(renderer, gapSizeX + finalCellSize * 13.f, gapSizeY, finalCellSize);
	game.AdditionalRender(renderer, gapSizeX - finalCellSize, gapSizeY - finalCellSize, finalCellSize);
	game.AdditionalRender(renderer, gapSizeX + finalCellSize * 12.f, gapSizeY - finalCellSize, finalCellSize);
	
	SDL_RenderPresent(renderer);




	Sleep(40000);
	SDL_Quit();
	return 0;
}
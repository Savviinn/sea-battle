#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <Windows.h>
#include <iostream>
#include <string>

#include "Player.h"

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

	Player player1;
	Player player2;
	player1.InitPlayer(renderer, tileSheetSurface, 32, "map.png");
	player2.InitPlayer(renderer, tileSheetSurface, 32, "map.png");
	player1.SetTurn(true);

	player1.AttackPlayer(player2, 1, 3); //=B4
	player1.AttackPlayer(player2, 3, 4);
	player1.AttackPlayer(player2, 1, 1);

	player2.SetTurn(true);
	player2.AttackPlayer(player1, 4, 8); //=E9
	player2.AttackPlayer(player1, 2, 3);

	vector<Player> pps = { player1, player2 };

	for (auto& p : pps) {
		string finalString = "";
		for (auto& row : p.GetTileLogic().GetTileArray()) {
			for (auto tile : row) {
				finalString += to_string(tile) + ' ';
			}
			finalString += '\n';
		}

		for (int row = 0; row < p.GetTileLogic().GetMapSize(); row++) {
			for (int column = 0; column < p.GetTileLogic().GetMapSize(); column++) {
				finalString += to_string(p.GetTileLogic().GetTile(row, column)) + ' ';
			}
			finalString += '\n';
		}
		cout << finalString << endl;
	}



	Sleep(10000);
	SDL_Quit();
	return 0;
}
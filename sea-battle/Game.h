#pragma once

#include "Player.h"

class Game
{
private:
	Player player1;
	Player player2;
	TileList additionalTileSheet;
	bool isPlayer1Turn;

	void SwitchTurn();
	void AdditionalRenderTile(SDL_Renderer* renderer, int index, float offsetX, float offsetY, float finalCellSize);
public:
	Game();
	bool InitGame(SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* player1FileName = nullptr, const char* player2FileName = nullptr);
	bool LoadAdditionalTileSheet(SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName = nullptr);
	void AdditionalRender(SDL_Renderer* renderer, float offsetX = NULL, float offsetY = NULL, float finalCellSize = NULL);


	
	Player GetPlayer1() { return player1; }
	Player GetPlayer2() { return player2; }
};


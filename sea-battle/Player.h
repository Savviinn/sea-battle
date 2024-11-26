#pragma once

#include "TileLogic.h"
#include "TileList.h"

class Player
{
private:
	TileLogic tileLogic;
	TileList tileList;
	bool isTurn;
public:
	Player();

	bool InitPlayer(SDL_Renderer* renderer, SDL_Surface* surface, int cellSize, const char* fileName = nullptr);
	TileLogic GetTileLogic();
	TileList GetTileList();
	int AttackPlayer(Player& player, int row, int column);
	void Render(SDL_Renderer* renderer, int offsetX, int offsetY);
	void SetTurn(bool turn);
	const bool IsTurn() const;
	const bool HasLost() const;
};


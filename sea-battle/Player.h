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
	int AttackOpponent(Player& opponent, int x, int y);
	void Render(SDL_Renderer* renderer, int offsetX, int offsetY);
	void SetTurn(bool turn);
	bool IsTurn();
	bool HasLost();
};


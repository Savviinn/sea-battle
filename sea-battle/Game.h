#pragma once

#include "Player.h"

class Game
{
private:
	Player player1;
	Player player2;
	bool isPlayer1Turn;

	void SwitchTurn();
public:
	Game();
	bool InitGame(SDL_Renderer* renderer, SDL_Surface* surface, int cellSize, const char* player1FileName = nullptr, const char* player2FileName = nullptr);
	Player GetPlayer1() { return player1; }
	Player GetPlayer2() { return player2; }
};


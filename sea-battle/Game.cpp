#include "Game.h"

Game::Game() : isPlayer1Turn(true), player1(), player2() {}

bool Game::InitGame(SDL_Renderer* renderer, SDL_Surface* surface, int cellSize, const char* player1FileName, const char* player2FileName) {
	if (
		!player1.InitPlayer(renderer, surface, cellSize, player1FileName) 
	    || !player2.InitPlayer(renderer, surface, cellSize, player2FileName)
	) 
    { return false; }

	return true;
}

void Game::SwitchTurn() {
	isPlayer1Turn = !isPlayer1Turn;
}
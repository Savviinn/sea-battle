#pragma once

#include "Player.h"

class Game
{
private:
	Player player1;
	Player player2;
	TileList markingTileSheet;
	TileList gridTileSheet;


	void RenderMarkingTile(SDL_Renderer* renderer, int index, float offsetX, float offsetY, float finalCellSize);
	
	//void RenderOpponent(SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize);
	void RenderPlayer(Player& player, SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize);
	void RenderMarking(SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize);
	void RenderGrid(   SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize);
public:
	Game();
	bool InitGame(			  SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName = nullptr);
	bool LoadMarkingTileSheet(SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName = nullptr);
	bool LoadGridTileSheet(	  SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName = nullptr);
	
	void Render(SDL_Renderer* renderer, float offsetX = NULL, float offsetY = NULL, float finalCellSize = NULL);

	void RandomizeShipLayout();

	bool AttackOpponentTile(int row, int column);

	//Player& GetPlayer1() { return player1; }
	//Player& GetPlayer2() { return player2; }
	void SwitchTurn();
	const bool IsPlayer1Turn() const;
	const bool IsWon() const;
};


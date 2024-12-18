#pragma once

#include <random>

#include "TileLogic.h"
#include "TileList.h"

class Player
{
private:
	TileLogic tileLogic;
	TileList tileList;
	bool isTurn;

	void RenderTile(SDL_Renderer* renderer, int tileValue, float offsetX, float offsetY, float finalCellSize);
	const float CalculateFinalSize(float finalCellSize) const;
public:
	Player();

	bool InitPlayer(SDL_Renderer* renderer, SDL_Surface* surface, int cellSize, const char* fileName = nullptr);
	SDL_Texture* GetTileSheet() const;
	
	bool PlaceShip(int startRow, int startCol, int length, bool isHorizontal);
	void RandomizeShipLayout();
	
	
	void Render(SDL_Renderer* renderer, float offsetX = NULL, float offsetY = NULL, float finalCellSize = NULL);
	void RenderAttacked(SDL_Renderer* renderer, float offsetX = NULL, float offsetY = NULL, float finalCellSize = NULL);
	
	void RenderSpecificTile(SDL_Renderer* renderer, int row, int col, float offsetX = NULL, float offsetY = NULL, float finalCellSize = NULL);
	void RenderAttackedTile(SDL_Renderer* renderer, int row, int col, float offsetX, float offsetY, float finalCellSize);
	
	int AttackPlayerTile(Player& player, int row, int column);

	void SetTurn(bool turn);
	const bool IsTurn() const;
	
	const bool HasLost() const;
};


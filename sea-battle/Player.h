#pragma once

#include "TileLogic.h"
#include "TileList.h"

class Player
{
private:
	TileLogic tileLogic;
	TileList tileList;
	bool isTurn;

	void RenderTile(SDL_Renderer* renderer, int row, int col, float offsetX, float offsetY, float finalCellSize);
public:
	Player();

	bool InitPlayer(SDL_Renderer* renderer, SDL_Surface* surface, int cellSize, const char* fileName = nullptr);

	SDL_Texture* GetTileSheet();
	int AttackPlayer(Player& player, int row, int column);
	void Render(SDL_Renderer* renderer, float offsetX = NULL, float offsetY = NULL, float finalCellSize = NULL);
	void RenderPositionedTile(SDL_Renderer* renderer, int row, int col, float offsetX = NULL, float offsetY = NULL, float finalCellSize = NULL);
	void SetTurn(bool turn);
	const bool IsTurn() const;
	const bool HasLost() const;
};


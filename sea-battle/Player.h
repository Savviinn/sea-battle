#pragma once

#include <random>

#include "TileLogic.h"
#include "TileList.h"
#include "Statistics.h"

class Player
{
private:
	StatsInfo playerInfo;

	TileLogic tileLogic;
	TileList tileList;
	bool isTurn;

	SDL_Texture* GetTileSheet() const;

	void RenderTile(SDL_Renderer* renderer, int tileValue, float offsetX, float offsetY, float finalCellSize);
	//void RenderSpecificTile(SDL_Renderer* renderer, int row, int col, float offsetX = NULL, float offsetY = NULL, float finalCellSize = NULL);
	//void RenderAttackedTile(SDL_Renderer* renderer, int row, int col, float offsetX, float offsetY, float finalCellSize);
	
	const float CalculateFinalSize(float finalCellSize) const;

	bool PlaceShip(int startRow, int startCol, int length, bool isHorizontal);
public:
	Player();
	bool InitPlayer(SDL_Renderer* renderer, SDL_Surface* surface, int cellSize, const char* fileName = nullptr);
	
	const string GetName() const;

	const int GetWinCount() const;
	void	  IncreaseWinCount();
	const int GetLoseCount() const;
	void	  IncreaseLoseCount();

	void LoadStatistics(const string& filePath, string& playerName);
	void SaveStatistics(const string& filePath);
	
	void RandomizeShipLayout();
	
	void Render(SDL_Renderer* renderer, float offsetX = NULL, float offsetY = NULL, float finalCellSize = NULL);
	void RenderAttacked(SDL_Renderer* renderer, float offsetX = NULL, float offsetY = NULL, float finalCellSize = NULL);
	
    int AttackPlayerTile(Player& player, int row, int column);
	bool IsAttackedTile(int row, int column);
	bool IsShipDestroyed(int row, int column);

	void SetTurn(bool turn);
	const bool IsTurn() const;
	const bool HasLost() const;
};


#pragma once

#include <SDL3/SDL_ttf.h>

#include "Player.h"
#include "Bot.h"

class Game
{
private:
	Player player;
	Bot playerBot;
	TileList markingTileSheet;
	TileList gridTileSheet;

	string statsFilePath;

	bool canReceiveStats;

	void RenderMarkingTile(SDL_Renderer* renderer, int index, float offsetX, float offsetY, float finalCellSize);
	
	void RenderPlayer(Player& player, SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize);
	void RenderMarking(SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize);
	void RenderGrid(   SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize);
	void SwitchTurn();
public:
	Game();
	bool InitGame(			  SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName = nullptr);
	bool LoadMarkingTileSheet(SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName = nullptr);
	bool LoadGridTileSheet(	  SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName = nullptr);
	bool LoadStatistic(const string& filePath, string& playerName);

	void Render(SDL_Renderer* renderer, bool isPlayer = true, float offsetX = NULL, float offsetY = NULL, float finalCellSize = NULL);
	void RenderText(SDL_Renderer* renderer, TTF_Font* font, const string& text, SDL_Color color, float offsetX = NULL, float offsetY = NULL);

	void RandomizeShipLayout(bool isPlayer);

	bool AttackBotTile(int row, int column);
	bool BotAttackCycle();

	const bool IsPlayerTurn() const;
	const int IsWon();
	void RestartGame();
	void SaveStats();
};


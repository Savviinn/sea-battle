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

	void RenderMarkingTile(SDL_Renderer* renderer, int index, float offsetX, float offsetY, float finalCellSize);
	
	void RenderPlayer(Player& player, SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize);
	void RenderMarking(SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize);
	void RenderGrid(   SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize);
public:
	Game();
	bool InitGame(			  SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName = nullptr);
	bool LoadMarkingTileSheet(SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName = nullptr);
	bool LoadGridTileSheet(	  SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName = nullptr);
	
	void Render(SDL_Renderer* renderer, float offsetX = NULL, float offsetY = NULL, float finalCellSize = NULL);
	void RenderText(SDL_Renderer* renderer, float fontSize, TTF_Font* font, const string& text, SDL_Color color, float offsetX = NULL, float offsetY = NULL);

	void RandomizeShipLayout();

	bool AttackBotTile(int row, int column);
	bool BotAttackCycle();

	void SwitchTurn();
	const bool IsPlayerTurn() const;
	const int IsWon() const;
};


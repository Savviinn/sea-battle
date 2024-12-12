#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

class TileList
{
private:
	SDL_Texture* tileSheet;
	vector<SDL_FRect> tiles;
	int cellsCount;
	
public:
	TileList();

	bool LoadTileSheet(SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName = nullptr);
	SDL_Texture* GetTileSheet() const;

	const SDL_FRect GetTileRect(int character) const;

	const int GetCellsCount() const;
	~TileList()
	{
		if (tileSheet != nullptr) {
			SDL_DestroyTexture(tileSheet);
			tileSheet = nullptr;
		}
		tiles.clear();
	}
};


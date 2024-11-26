#include "TileList.h"
#include <iostream>
#include <string>

TileList::TileList() : tileSheet(nullptr), cellsCount(0) {}

bool TileList::LoadTileSheet(SDL_Renderer* renderer, SDL_Surface* surface, int cellSize, const char* fileName) {
	if (!renderer) {
		cerr << "Invalid renderer for tile sheet" << endl;
		return false;
	}

	if (!surface && fileName) {
		string basePath = SDL_GetBasePath();
		string filePath = basePath + "tiles/" + fileName;

		surface = IMG_Load(filePath.c_str());
		if (!surface) {
			cerr << "Failed to load image: " << SDL_GetError() << endl;
			return false;
		}
	}
	else if (!surface) {
		cerr << "No provided file for surface" << endl;
		return false;
	}

	tileSheet = SDL_CreateTextureFromSurface(renderer, surface);
	if (!tileSheet) {
		cerr << "Failed to create texture from surface: " << SDL_GetError() << endl;
		return false;
	}

	int imageWidth = surface->w;
	int imageHeight = surface->h;
	int cellCountInColumn = static_cast<int>(floor(imageWidth / cellSize));
	int cellCountInRow = static_cast<int>(floor(imageHeight / cellSize));

	cellsCount = cellCountInColumn * cellCountInRow;
	tiles.clear();

	for (int row = 0; row < cellCountInColumn; row++) {
		for (int column = 0; column < cellCountInRow; column++) {
			SDL_FRect srcRect = {
				static_cast<float>(row * cellSize),
				static_cast<float>(column * cellSize),
				static_cast<float>(cellSize),
				static_cast<float>(cellSize)
			};
			tiles.emplace_back(srcRect);
		}
	}

	SDL_DestroySurface(surface);
	return true;
}

SDL_Texture* TileList::GetTileSheet() {
	return tileSheet;
}
SDL_FRect		  TileList::GetTileRect(int character) {
	if (character < 0 || character >= cellsCount) {
		cerr << "Invalid character index for tile" << endl;
		return { 0, 0, 0, 0 };
	}
	return tiles[character];
}
vector<SDL_FRect>& TileList::GetTileList() {
	return tiles;
}

const int TileList::GetCellsCount() const {
	return cellsCount;
}
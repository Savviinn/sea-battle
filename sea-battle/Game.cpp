#include "Game.h"

Game::Game() : isPlayer1Turn(true), player1(), player2() {}

bool Game::InitGame(SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* player1FileName, const char* player2FileName) {
	return 
		player1.InitPlayer(renderer, surface, cellSize, player1FileName) 
		&& player2.InitPlayer(renderer, surface, cellSize, player2FileName);
}

bool Game::LoadAdditionalTileSheet(SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName) {
	return additionalTileSheet.LoadTileSheet(renderer, surface, cellSize, fileName);
}

void Game::AdditionalRenderTile(SDL_Renderer* renderer, int index, float offsetX, float offsetY, float finalCellSize) {
	auto tileRect = this->additionalTileSheet.GetTileRect(index);
	if (tileRect.w <= 0 || tileRect.h <= 0) {
		cerr << "Invalid texture rectangle for additional tile: " << index << endl;
		return;
	}

	SDL_FRect dstRect = {
		offsetX,
		offsetY,
		finalCellSize,
		finalCellSize
	};

	if (!SDL_RenderTexture(renderer, this->additionalTileSheet.GetTileSheet(), &tileRect, &dstRect)) {
		cerr << "SDL_RenderTexture occured: " << SDL_GetError() << endl;
		return;
	}
}

void Game::AdditionalRender(SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize) {
	if (!renderer) {
		cerr << "Invalid renderer for tile sheet" << endl;
		return;
	}

	for (int i = 0; i < this->additionalTileSheet.GetCellsCount(); i++) {
		if (i < 10) {
			AdditionalRenderTile(
				renderer, 
				i, 
				offsetX + finalCellSize + finalCellSize * i,
				offsetY, 
				finalCellSize
			);
		}
		else if (i < 20) {
			AdditionalRenderTile(
				renderer, 
				i, 
				offsetX, 
				offsetY + finalCellSize + finalCellSize * (i%10),
				finalCellSize
			);
		}
	}
}



void Game::SwitchTurn() {
	isPlayer1Turn = !isPlayer1Turn;
}
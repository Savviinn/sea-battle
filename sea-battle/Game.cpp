#include "Game.h"

Game::Game() : player1(), player2() {}

bool Game::InitGame(SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName) {
	player1.SetTurn(true);
	return 
		player1.InitPlayer(renderer, surface, cellSize, fileName) 
		&& player2.InitPlayer(renderer, surface, cellSize, fileName);
}

bool Game::LoadAdditionalTileSheet(SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName) {
	return additionalTileSheet.LoadTileSheet(renderer, surface, cellSize, fileName);
}

//void Game::RenderOpponent(SDL_Renderer* renderer, Player& player, float offsetX, float offsetY, float finalCellSize) {
//	int mapSize = TileLogic::GetMapSize();
//	if (mapSize <= 0) {
//		cerr << "Invalid map size" << endl;
//		return;
//	}
//	if (!renderer) {
//		cerr << "Renderer is invalid" << endl;
//		return;
//	}
//	if (!player.GetTileSheet()) {
//		cerr << "Get tile sheet error occured: " << endl;
//		return;
//	}
//
//	float finalSize = finalCellSize != NULL ? finalCellSize : player.tileList.GetTileRect(0).h;
//
//	for (int row = 0; row < mapSize; row++) {
//		for (int col = 0; col < mapSize; col++) {
//			if (attackedTile[row][col]) {
//				RenderPositionedTile(renderer, row, col, offsetX, offsetY, finalSize);
//			}
//			else {
//				RenderHiddenTile(renderer, row, col, offsetX, offsetY, finalSize);
//			}
//		}
//	}
//}

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
	player1.SetTurn(!player1.IsTurn());
	player2.SetTurn(!player2.IsTurn());
}
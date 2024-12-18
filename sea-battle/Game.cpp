#include "Game.h"

Game::Game() : player1(), player2() {}

bool Game::InitGame(SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName) {
	player1.SetTurn(true);
	//if (
	//	!player1.InitPlayer(renderer, surface, cellSize, fileName)
	//	&& !player2.InitPlayer(renderer, surface, cellSize, fileName)
	//) return false;
	return 
		player1.InitPlayer(renderer, surface, cellSize, fileName) &&
		player2.InitPlayer(renderer, surface, cellSize, fileName);
}

bool Game::LoadMarkingTileSheet(SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName) {
	return markingTileSheet.LoadTileSheet(renderer, surface, cellSize, fileName);
}

bool Game::LoadGridTileSheet(SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName) {
	return gridTileSheet.LoadTileSheet(renderer, surface, cellSize, fileName);
}

void Game::RandomizeShipLayout() {
	auto& player = player1.IsTurn() ? player1 : player2;
	player.RandomizeShipLayout();
	SwitchTurn();
}

void Game::RenderMarkingTile(SDL_Renderer* renderer, int index, float offsetX, float offsetY, float finalCellSize) {
	auto tileRect = this->markingTileSheet.GetTileRect(index);
	if (tileRect.w <= 0 || tileRect.h <= 0) {
		cerr << "Invalid texture rectangle for marking tile: " << index << endl;
		return;
	}

	SDL_FRect dstRect = {
		offsetX,
		offsetY,
		finalCellSize,
		finalCellSize
	};

	if (!SDL_RenderTexture(renderer, this->markingTileSheet.GetTileSheet(), &tileRect, &dstRect)) {
		cerr << "SDL_RenderTexture occured: " << SDL_GetError() << endl;
		return;
	}
}

void Game::RenderPlayer(Player& player, SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize) {
	if (player.IsTurn()) {
		player.Render(renderer, offsetX, offsetY, finalCellSize);
	}
	else {
		player.RenderAttacked(renderer, offsetX, offsetY, finalCellSize);
	}
}

void Game::RenderMarking(SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize) {
	if (!renderer) {
		cerr << "Invalid renderer for tile sheet" << endl;
		return;
	}

	for (int i = 0; i < this->markingTileSheet.GetCellsCount(); i++) {
		if (i < 10) {
			RenderMarkingTile(
				renderer, 
				i, 
				offsetX + finalCellSize + finalCellSize * i,
				offsetY, 
				finalCellSize
			);
		}
		else if (i < 20) {
			RenderMarkingTile(
				renderer, 
				i, 
				offsetX, 
				offsetY + finalCellSize + finalCellSize * (i%10),
				finalCellSize
			);
		}
	}
}

void Game::RenderGrid(SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize) {
	if (!renderer) {
		cerr << "Invalid renderer for grid" << endl;
		return;
	}

	auto tileRect = this->gridTileSheet.GetTileRect(0);
	if (tileRect.w <= 0 || tileRect.h <= 0) {
		cerr << "Invalid texture rectangle for grid tile: " << endl;
		return;
	}

	SDL_FRect dstRect = {
		offsetX,
		offsetY,
		finalCellSize,
		finalCellSize
	};

	if (!SDL_RenderTexture(renderer, this->gridTileSheet.GetTileSheet(), &tileRect, &dstRect)) {
		cerr << "SDL_RenderTexture occured: " << SDL_GetError() << endl;
		return;
	}
}

void Game::Render(SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize) {
	if (!renderer) {
		cerr << "Invalid renderer for game" << endl;
		return;
	}

	auto& player1 = this->player1;
	auto& player2 = this->player2;

	RenderPlayer(player1, renderer, offsetX, offsetY, finalCellSize);
	RenderMarking(renderer, offsetX - finalCellSize, offsetY - finalCellSize, finalCellSize     );
	RenderGrid(   renderer, offsetX - finalCellSize, offsetY - finalCellSize, finalCellSize * 11);	
	
	RenderPlayer(player2, renderer, offsetX + finalCellSize * 12.f, offsetY, finalCellSize);
	RenderMarking(renderer, offsetX + finalCellSize * 11.f, offsetY - finalCellSize, finalCellSize	   );
	RenderGrid(   renderer, offsetX + finalCellSize * 11.f, offsetY - finalCellSize, finalCellSize * 11);
}

bool Game::AttackOpponentTile(int row, int column) {
	auto& player   = player1.IsTurn() ? player1 : player2;
	auto& opponent = player1.IsTurn() ? player2 : player1;

	auto result = player.AttackPlayerTile(opponent, row, column);
	if (result == 3 || result == -2 || result == -1) {
		return true;
	}

	SwitchTurn();
	return false;
}

void Game::SwitchTurn() {
	player1.SetTurn(!player1.IsTurn());
	player2.SetTurn(!player2.IsTurn());
}

const bool Game::IsPlayer1Turn() const {
	return player1.IsTurn();
}

const bool Game::IsWon() const {
	return player1.HasLost() || player2.HasLost();
}


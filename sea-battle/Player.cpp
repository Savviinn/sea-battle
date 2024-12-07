#include "Player.h"


Player::Player() : isTurn(false) {}

bool Player::InitPlayer(SDL_Renderer* renderer, SDL_Surface* tilesSurface, int cellSize, const char* fileName) {
	return tileLogic.InitLogic() && tileList.LoadTileSheet(renderer, tilesSurface, cellSize, fileName);
}

SDL_Texture* Player::GetTileSheet() {
	return tileList.GetTileSheet();
}

int Player::AttackPlayer(Player& player, int row, int column) {
	if (!isTurn) {
		return -1;
	}
	int attackResult = player.tileLogic.AttackTile(row, column);
	if (attackResult != 3) {
		isTurn = false;
	}
	return attackResult;
}

void Player::RenderTile(SDL_Renderer* renderer, int row, int col, float offsetX, float offsetY, float finalCellSize) {
	int tileValue = this->tileLogic.GetTile(row, col);
	auto tileRect = this->tileList.GetTileRect(tileValue);
	if (tileRect.w <= 0 || tileRect.h <= 0) {
		cerr << "Invalid texture rectangle for tile: " << tileValue << endl;
		return;
	}

	SDL_FRect dstRect = {
		static_cast<float>(offsetX + col * finalCellSize),
		static_cast<float>(offsetY + row * finalCellSize),
		finalCellSize,
		finalCellSize
	};

	if (!SDL_RenderTexture(renderer, this->GetTileSheet(), &tileRect, &dstRect)) {
		cerr << "SDL_RenderTexture occured: " << SDL_GetError() << endl;
	}
}

void Player::Render(SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize) {
	int mapSize = TileLogic::GetMapSize();
	if (mapSize <= 0) {
		cerr << "Invalid map size" << endl;
		return;
	}
	if (!renderer) {
		cerr << "Renderer is invalid" << endl;
		return;
	}
	if (!this->GetTileSheet()) {
		cerr << "Get tile sheet error occured: " << endl;
		return;
	}
	else {
		cout << "Using " << this->GetTileSheet() << " tile sheet" << endl;
	}

	float finalSize = finalCellSize != NULL ? finalCellSize : this->tileList.GetTileRect(0).h;

	for (int row = 0; row < mapSize; ++row) {
		for (int col = 0; col < mapSize; ++col) {
			RenderTile(renderer, row, col, offsetX, offsetY, finalSize);
		}
	}
}

void Player::RenderPositionedTile(SDL_Renderer* renderer, int row, int col, float offsetX, float offsetY, float finalCellSize) {
	if (TileLogic::GetMapSize() <= 0) {
		cerr << "Invalid map size" << endl;
		return;
	}
	if (!renderer) {
		cerr << "Renderer is invalid" << endl;
		return;
	}
	if (!this->GetTileSheet()) {
		cerr << "Get tile sheet error occured: " << endl;
		return;
	}

	float finalSize = finalCellSize != NULL ? finalCellSize : this->tileList.GetTileRect(0).h;
	
	RenderTile(renderer, row, col, offsetX, offsetY, finalSize);
}

void Player::SetTurn(bool turn) {
	isTurn = turn;
}

const bool Player::IsTurn() const{
	return isTurn;
}

const bool Player::HasLost() const {
	return tileLogic.HasLost();
}

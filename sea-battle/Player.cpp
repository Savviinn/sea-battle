#include "Player.h"
#include <iostream>

Player::Player() : isTurn(false), tileLogic(), tileList() {}

bool Player::InitPlayer(SDL_Renderer* renderer, SDL_Surface* tilesSurface, int cellSize, const char* fileName) {
	if (!tileLogic.InitLogic()) {
		cerr << "Init Logic error occured" << endl;
		return false;
	}
	if (!tileList.LoadTileSheet(renderer, tilesSurface, cellSize, fileName)) {
		cerr << "Load Tile Sheet error occured" << endl;
		return false;
	}
	return true;
}

TileLogic& Player::GetTileLogic() {
	return tileLogic;
}

TileList& Player::GetTileList() {
	return tileList;
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

void Player::Render(SDL_Renderer* renderer, int offsetX, int offsetY, float finalCellSize) {
	int mapSize = TileLogic::GetMapSize();

	float finalSize = finalCellSize != NULL ? finalCellSize / mapSize : tileList.GetTileRect(0).h;

	for (int row = 0; row < mapSize; ++row) {
		for (int column = 0; column < mapSize; ++column) {
			int tileValue = tileLogic.GetTile(row, column);
			SDL_FRect tileRect = tileList.GetTileRect(tileValue);

			SDL_FRect destRect = {
				static_cast<float>(offsetX + row * finalSize),
				static_cast<float>(offsetY + column * finalSize),
				finalSize,
				finalSize
			};
			cout << "Rendering tile: " << tileValue << " at (" << destRect.x << ", " << destRect.y << ")" << endl;

			if (!SDL_RenderTexture(renderer, tileList.GetTileSheet(), &tileRect, &destRect)) {
				cerr << "SDL_RenderTexture error occured: " << SDL_GetError() << endl;
				return;
			}
		}
	}
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

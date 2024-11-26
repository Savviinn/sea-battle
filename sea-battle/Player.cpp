#include "Player.h"

Player::Player() : isTurn(false) {}

bool Player::InitPlayer(SDL_Renderer* renderer, SDL_Surface* tilesSurface, int cellSize, const char* fileName) {
	if (!tileLogic.InitLogic()) {
		return false;
	}

	return tileList.LoadTileSheet(renderer, tilesSurface, cellSize, fileName);
}

TileLogic Player::GetTileLogic() {
	return tileLogic;
}

TileList Player::GetTileList() {
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

void Player::Render(SDL_Renderer* renderer, int offsetX, int offsetY) {
	auto tiles = tileList.GetTileList();
	int mapSize = TileLogic::GetMapSize();

	for (int row = 0; row < mapSize; ++row) {
		for (int column = 0; column < mapSize; ++column) {
			int tileValue = tileLogic.GetTile(row, column);
			SDL_FRect tileRect = tileList.GetTileRect(tileValue);

			SDL_FRect destRect = {
				static_cast<float>(offsetX + row * tileRect.w),
				static_cast<float>(offsetY + column * tileRect.h),
				tileRect.w,
				tileRect.h
			};

			SDL_RenderTexture(renderer, tileList.GetTileSheet(), &tileRect, &destRect);
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

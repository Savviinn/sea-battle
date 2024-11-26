#include "Player.h"

Player::Player() : isTurn(false) {}

bool Player::InitPlayer(SDL_Renderer* renderer, SDL_Surface* tilesSurface, int cellSize, const char* fileName) {
	if (!tileLogic.InitLogic()) {
		return false;
	}

	return tileList.LoadTileSheet(renderer, tilesSurface, cellSize, fileName);
}

int Player::AttackOpponent(Player& opponent, int x, int y) {
	if (!isTurn) {
		return -1;
	}
	int attackResult = opponent.tileLogic.AttackTile(x, y);
	if (attackResult != 3) {
		isTurn = false;
	}
	return attackResult;
}

bool Player::HasLost() {
	return tileLogic.HasLost();
}

void Player::Render(SDL_Renderer* renderer, int offsetX, int offsetY) {
	auto tiles = tileList.GetTileList();
	int mapSize = TileLogic::GetMapSize();

	for (int x = 0; x < mapSize; ++x) {
		for (int y = 0; y < mapSize; ++y) {
			int tileValue = tileLogic.GetTile(x, y);
			SDL_FRect tileRect = tileList.GetTileRect(tileValue);

			SDL_FRect destRect = {
				static_cast<float>(offsetX + x * tileRect.w),
				static_cast<float>(offsetY + y * tileRect.h),
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

bool Player::IsTurn() {
	return isTurn;
}
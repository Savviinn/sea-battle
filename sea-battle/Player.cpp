#include "Player.h"

const float Player::CalculateFinalSize(float finalCellSize) const {
	return finalCellSize != NULL ? finalCellSize : this->tileList.GetTileRect(0).h;
}


Player::Player() : isTurn(false) {}

bool Player::InitPlayer(SDL_Renderer* renderer, SDL_Surface* tilesSurface, int cellSize, const char* fileName) {
	return this->tileLogic.InitLogic() && this->tileList.LoadTileSheet(renderer, tilesSurface, cellSize, fileName);
}

SDL_Texture* Player::GetTileSheet() const{
	return this->tileList.GetTileSheet();
}

bool Player::PlaceShip(int startRow, int startCol, int length, bool isHorizontal) {
	return this->tileLogic.PlaceShip(startRow, startCol, length, isHorizontal);
}

void Player::RandomizeShipLayout() {
	int mapSize = TileLogic::GetMapSize();

	vector<pair<int, int>> shipConfig = {
		{4, 1},
		{3, 2},
		{2, 3},
		{1, 4}
	};

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> coordDist(0, mapSize - 1);
	uniform_int_distribution<> dirDist(0, 1);

	this->tileLogic.ResetTileArray();

	for (const auto& [length, count] : shipConfig) {
		int remainingShips = count;
		while (remainingShips > 0) {
			int  startRow     = coordDist(gen);
			int  startCol     = coordDist(gen);
			bool isHorizontal = static_cast<bool>(dirDist(gen));

			if (this->tileLogic.PlaceShip(startRow, startCol, length, isHorizontal)) {
				remainingShips--;
			}
		}
	}
}

int Player::AttackPlayerTile(Player& player, int row, int column) {
	if (!isTurn || this == &player) {
		return -1;
	}
	return player.tileLogic.AttackTile(row, column);
}

void Player::RenderTile(SDL_Renderer* renderer, int tileValue, float offsetX, float offsetY, float finalCellSize) {
	auto tileRect = this->tileList.GetTileRect(tileValue);
	if (tileRect.w <= 0 || tileRect.h <= 0) {
		cerr << "Invalid texture rectangle for tile: " << tileValue << endl;
		return;
	}

	SDL_FRect dstRect = {
		offsetX,
		offsetY,
		finalCellSize,
		finalCellSize
	};

	if (!SDL_RenderTexture(renderer, this->GetTileSheet(), &tileRect, &dstRect)) {
		cerr << "SDL_RenderTexture occured: " << SDL_GetError() << endl;
	}
}

void Player::Render(SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize) {
	int mapSize = TileLogic::GetMapSize();
	if (!renderer) {
		cerr << "Renderer is invalid" << endl;
		return;
	}
	if (!this->GetTileSheet()) {
		cerr << "Get tile sheet error occured: " << endl;
		return;
	}

	float finalSize = CalculateFinalSize(finalCellSize);

	for (int row = 0; row < mapSize; row++) {
		for (int col = 0; col < mapSize; col++) {
			RenderTile(
				renderer, 
				this->tileLogic.GetTile(row, col),
				offsetX + col * finalCellSize, 
				offsetY + row * finalCellSize, 
				finalSize
			);
		}
	}
}

void Player::RenderSpecificTile(SDL_Renderer* renderer, int row, int col, float offsetX, float offsetY, float finalCellSize) {
	int mapSize = TileLogic::GetMapSize();
	if (row < 0 || row >= mapSize || col < 0 || col >= mapSize) {
		cerr << "Invalid map size or coordinates" << endl;
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

	float finalSize = CalculateFinalSize(finalCellSize);

	RenderTile(
		renderer, 
		this->tileLogic.GetTile(row, col),
		offsetX + col * finalCellSize, 
		offsetY + row * finalCellSize, 
		finalSize
	);
}

void Player::RenderAttacked(SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize) {
	int mapSize = TileLogic::GetMapSize();
	if (!renderer) {
		cerr << "Renderer is invalid" << endl;
		return;
	}
	if (!this->GetTileSheet()) {
		cerr << "Get tile sheet error occured: " << endl;
		return;
	}

	float finalSize = CalculateFinalSize(finalCellSize);
	for (int row = 0; row < mapSize; row++) {
		for (int col = 0; col < mapSize; col++) {
			auto tileValue = this->tileLogic.GetAttackedTile(row, col) ? this->tileLogic.GetTile(row, col) : 0;
			RenderTile(
				renderer,
				tileValue,
				offsetX + col * finalCellSize,
				offsetY + row * finalCellSize,
				finalSize
			);
		}
	}
}


void Player::RenderAttackedTile(SDL_Renderer* renderer, int row, int col, float offsetX, float offsetY, float finalCellSize) {
	if (!renderer) {
		cerr << "Renderer is invalid" << endl;
		return;
	}
	if (!this->GetTileSheet()) {
		cerr << "Get tile sheet error occured: " << endl;
		return;
	}

	float finalSize = CalculateFinalSize(finalCellSize);
	auto tileValue = this->tileLogic.GetAttackedTile(row, col) 
		? this->tileLogic.GetTile(row, col) 
		: 0;

	RenderTile(
		renderer,
		tileValue,
		offsetX + col * finalCellSize,
		offsetY + row * finalCellSize,
		finalSize
	);
}

void Player::SetTurn(bool turn) {
	isTurn = turn;
}

const bool Player::IsTurn() const{
	return isTurn;
}

const bool Player::HasLost() const {
	for (auto& row : this->tileLogic.GetTileArray()) {
		for (int tile : row) {
			if (tile == 2) {
				return false;
			}
		}
	}
	return true;
}

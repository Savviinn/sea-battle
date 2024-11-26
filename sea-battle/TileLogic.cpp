#include "TileLogic.h"

TileLogic::TileLogic() {}

bool TileLogic::InitLogic() {
	attackedTileArray = vector<vector<bool>>(mapSize, vector<bool>(mapSize, false));
	mapArray = vector<vector<int>>(mapSize, vector<int>(mapSize, 0));
	return !(mapArray.empty() || attackedTileArray.empty());
}

int TileLogic::AttackTile(int row, int column) {
	if (row < 0 || row >= mapSize || column < 0 || column >= mapSize) {
		return -1;
	}
	if (attackedTileArray[row][column] == true) {
		return -2;
	}
	attackedTileArray[row][column] = true;
	mapArray[row][column] += static_cast<int>(attackedTileArray[row][column]);

	return mapArray[row][column];
}

int TileLogic::GetTile(int row, int column) {
	if (row < 0 || row >= mapSize || column < 0 || column >= mapSize) {
		return -1;
	}
	return mapArray[row][column];
}

vector<vector<int>>& TileLogic::GetTileArray() {
	return mapArray;
}

const bool TileLogic::HasLost() const {
	for (auto& row : mapArray) {
		for (int tile : row) {
			if (tile == 2) {
				return false;
			}
		}
	}
	return true;
}

const int TileLogic::GetMapSize() {
	return mapSize;
}
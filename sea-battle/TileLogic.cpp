#include "TileLogic.h"

TileLogic::TileLogic() {}

bool TileLogic::InitLogic() {
	attackedTileArray = vector<vector<bool>>(mapSize, vector<bool>(mapSize, false));
	mapArray = vector<vector<int>>(mapSize, vector<int>(mapSize, 0));
	return !(mapArray.empty() || attackedTileArray.empty());
}

int TileLogic::AttackTile(int x, int y) {
	if (x < 0 || x >= mapSize || y < 0 || y >= mapSize) {
		return -1;
	}
	if (attackedTileArray[x][y] == true) {
		return -2;
	}
	attackedTileArray[x][y] = true;
	mapArray[x][y] += static_cast<int>(attackedTileArray[x][y]);

	return mapArray[x][y];
}

int TileLogic::GetTile(int x, int y) {
	if (x < 0 || x >= mapSize || y < 0 || y >= mapSize) {
		return -1;
	}
	return mapArray[x][y];
}

vector<vector<int>> TileLogic::GetTileArray() {
	return mapArray;
}

bool TileLogic::HasLost() const {
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
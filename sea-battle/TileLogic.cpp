#include "TileLogic.h"

TileLogic::TileLogic() {}

bool TileLogic::InitLogic() {
	attackedTileArray = vector<vector<bool>>(mapSize, vector<bool>(mapSize, false));
	mapArray = vector<vector<int>>(mapSize, vector<int>(mapSize, 0));
	return !(mapArray.empty() || attackedTileArray.empty());
}

bool TileLogic::PlaceShip(int startRow, int startCol, int length, bool isHorizontal) {
	int mapSize = GetMapSize();
	if (mapSize <= 0 || length <= 0 || length > 4 || startRow < 0 || startCol < 0 || startRow >= mapSize || startCol >= mapSize) {
		return false;
	}
	if ((isHorizontal && startCol + length > mapSize) || (!isHorizontal && startRow + length > mapSize)) {
		return false;
	}

	int topRow = max(0, startRow - 1);
	int bottomRow = min(mapSize - 1, startRow + (isHorizontal ? 0 : length - 1) + 1);
	int leftCol = max(0, startCol - 1);
	int rightCol = min(mapSize - 1, startCol + (isHorizontal ? length - 1 : 0) + 1);

	for (int row = topRow; row <= bottomRow; row++) {
		for (int col = leftCol; col <= rightCol; col++) {
			if (mapArray[row][col] != 0) {
				return false;
			}
		}
	}

	for (int i = 0; i < length; i++) {
		mapArray[startRow + (isHorizontal ? 0 : i)][startCol + (isHorizontal ? i : 0)] = 2;
	}
	return true;
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

const int TileLogic::GetTile(int row, int column) const {
	if (row < 0 || row >= mapSize || column < 0 || column >= mapSize) {
		return -1;
	}
	return mapArray[row][column];
}

const vector<vector<int>> TileLogic::GetTileArray() const{
	return mapArray;
}

const bool TileLogic::GetAttackedTile(int row, int col) const {
	return attackedTileArray[row][col];
}


const int TileLogic::GetMapSize() {
	return mapSize;
}
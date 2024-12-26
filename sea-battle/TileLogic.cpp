#include "TileLogic.h"

TileLogic::TileLogic() {}

bool TileLogic::InitLogic() {
	attackedTileArray = vector<vector<bool>>(mapSize, vector<bool>(mapSize, false));
	tileArray = vector<vector<int>>(mapSize, vector<int>(mapSize, 0));
	return !(tileArray.empty() || attackedTileArray.empty());
}

void TileLogic::ResetTileArray() {
	for (auto& row : tileArray) {
		fill(row.begin(), row.end(), 0);
	}
	for (auto& row : attackedTileArray) {
		fill(row.begin(), row.end(), false);
	}
}


bool TileLogic::PlaceShip(int startRow, int startCol, int length, bool isHorizontal) {
	int mapSize = GetMapSize();
	if (length <= 0 || length > 4 || startRow < 0 || startCol < 0 || startRow >= mapSize || startCol >= mapSize) {
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
			if (tileArray[row][col] != 0) {
				return false;
			}
		}
	}

	for (int i = 0; i < length; i++) {
		tileArray[startRow + (isHorizontal ? 0 : i)][startCol + (isHorizontal ? i : 0)] = 2;
	}
	return true;
}

pair<bool, vector<pair<int, int>>> TileLogic::IsShipDestroyed(int row, int column) {
	if (tileArray[row][column] != 3) {
		return { false, {} };
	}

	vector<pair<int, int>> shipSearch = {
		{-1, 0}, {1, 0}, {0, -1}, {0, 1}
	};

	vector<pair<int, int>> destroyedShipCoords = {
		{row, column}
	};

	for (auto i = 0; i < destroyedShipCoords.size(); i++) {
		int currentRow = destroyedShipCoords[i].first;
		int currentCol = destroyedShipCoords[i].second;

		for (const auto& direction : shipSearch) {
			int searchRow = currentRow + direction.first;
			int searchCol = currentCol + direction.second;

			if (searchRow < 0 || searchRow >= mapSize || searchCol < 0 || searchCol >= mapSize) {
				continue;
			}

			if (tileArray[searchRow][searchCol] == 2) {
				return { false, {} };
			}

			if (tileArray[searchRow][searchCol] == 3 &&
				find(
					destroyedShipCoords.begin(),
					destroyedShipCoords.end(),
					make_pair(searchRow, searchCol)
				) == destroyedShipCoords.end()
				) {
				destroyedShipCoords.push_back({ searchRow, searchCol });
			}
		}
	}
	return { true, destroyedShipCoords };
}

void TileLogic::TryRevealTiles(int row, int column) {
	auto isDestroyed = IsShipDestroyed(row, column);

	if (isDestroyed.first) {
		for (const auto& coord : isDestroyed.second) {
			int currentRow = coord.first;
			int currentCol = coord.second;

			for (int dr = -1; dr <= 1; ++dr) {
				for (int dc = -1; dc <= 1; ++dc) {
					int revealRow = currentRow + dr;
					int revealCol = currentCol + dc;

					if (revealRow < 0 || revealRow >= mapSize || revealCol < 0 || revealCol >= mapSize) {
						continue;
					}

					if (tileArray[revealRow][revealCol] == 0) {
						attackedTileArray[revealRow][revealCol] = true;
						tileArray[revealRow][revealCol] += static_cast<int>(attackedTileArray[revealRow][revealCol]);
					}
				}
			}
		}
	}
}



int TileLogic::AttackTile(int row, int column) {
	if (row < 0 || row >= mapSize || column < 0 || column >= mapSize) {
		return -1;
	}
	if (attackedTileArray[row][column] == true) {
		return -2;
	}
	attackedTileArray[row][column] = true;
	tileArray[row][column] += static_cast<int>(attackedTileArray[row][column]);
	if (tileArray[row][column] == 3) {
		TryRevealTiles(row, column);
	}

	return tileArray[row][column];
}

const int TileLogic::GetTile(int row, int column) const {
	if (row < 0 || row >= mapSize || column < 0 || column >= mapSize) {
		return -1;
	}
	return tileArray[row][column];
}

const vector<vector<int>> TileLogic::GetTileArray() const{
	return tileArray;
}

const bool TileLogic::IsAttackedTile(int row, int col) const {
	return attackedTileArray[row][col];
}


const int TileLogic::GetMapSize() {
	return mapSize;
}
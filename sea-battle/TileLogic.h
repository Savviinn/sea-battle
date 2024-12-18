#pragma once

#include <vector>

using namespace std;

class TileLogic
{
private:
	vector<vector<int>> mapArray;
	vector<vector<bool>> attackedTileArray;
	static const int mapSize = 10;

	void TryRevealTiles(int row, int column);
public:
	TileLogic();
	bool InitLogic();
	void ResetTileArray();
	bool PlaceShip(int startRow, int startCol, int length, bool isHorizontal);
	int AttackTile(int row, int column);
	const int  GetTile(int row, int column) const;
	const bool GetAttackedTile(int row, int col) const;
	const vector<vector<int>> GetTileArray() const;

	static const int GetMapSize();
};


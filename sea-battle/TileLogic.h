#pragma once

#include <vector>

using namespace std;

class TileLogic
{
private:
	vector<vector<int>> mapArray;
	vector<vector<bool>> attackedTileArray;
	static const int mapSize = 10;
public:
	TileLogic();
	bool InitLogic();
	int AttackTile(int row, int column);
	int GetTile(int row, int column);
	vector<vector<int>>& GetTileArray();
	const bool HasLost() const;

	static const int GetMapSize();
};


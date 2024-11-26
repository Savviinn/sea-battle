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
	int AttackTile(int x, int y);
	int GetTile(int x, int y);
	vector<vector<int>> GetTileArray();
	bool HasLost() const;

	static const int GetMapSize();
};


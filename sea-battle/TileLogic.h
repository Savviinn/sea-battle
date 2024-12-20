#pragma once

#include <vector>

using namespace std;

class TileLogic
{
private:
	friend class Bot;

	vector<vector<int>> tileArray;
	vector<vector<bool>> attackedTileArray;
	static const int mapSize = 10;

	void TryRevealTiles(int row, int column);
	pair<bool, vector<pair<int, int>>> IsShipDestroyed(int row, int column);
protected:
	vector<vector<int>>& GetMapForBot() { return tileArray; }
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


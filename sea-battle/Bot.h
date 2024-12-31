#pragma once

#include "Player.h"

class Bot : public Player 
{
private:
	mt19937 gen;
	uniform_int_distribution<int> rdTileDistr;

	vector<pair<int, int>> attackDirections;
	
	bool hittingShip;
	vector<pair<int, int>> avaibleAttackDirections;
	vector<pair<int, int>> hittedShipCoords;

	pair<int, int> GetRandomCoords();
	pair<int, int> GetRandomDirection();
	
	int RandomAttackPlayer(Player& player);
	int AttackPlayer(Player& player);
public:
	Bot();

	int BotAttack(Player& player);
};


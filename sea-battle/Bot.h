#pragma once
#include "Player.h"

class Bot : public Player 
{
private:
	//int i;

	mt19937 gen;
	uniform_int_distribution<int> rdRowDistr;
	uniform_int_distribution<int> rdColDistr;

	vector<pair<int, int>> attackDirections;
	
	bool hittingShip;
	vector<pair<int, int>> avaibleAttackDirections;
	vector<pair<int, int>> hittedShipCoords;

	pair<int, int> GetRandomCoords();
	pair<int, int> GetRandomDirection();
public:
	Bot();

	int RandomAttackPlayer(Player& player);
	int AttackPlayer(Player& player);
	int BotAttack(Player& player);
};


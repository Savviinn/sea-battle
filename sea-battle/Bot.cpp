#include "Bot.h"

Bot::Bot() : rdRowDistr(0, TileLogic::GetMapSize() - 1), rdColDistr(0, TileLogic::GetMapSize()-1), gen(random_device{}()), hittingShip(false) {
	attackDirections = { {0, 1}, {1, 0}, {-1, 0}, {0, -1} };
	avaibleAttackDirections = {};
}

pair<int, int> Bot::GetRandomCoords() {
	return { rdRowDistr(gen), rdColDistr(gen) };
}

pair<int, int> Bot::GetRandomDirection() {
	if (avaibleAttackDirections.empty()) {
		uniform_int_distribution<int> distr(0, static_cast<int>(attackDirections.size()) - 1);
		return attackDirections[distr(gen)];
	}
	uniform_int_distribution<int> distr(0, static_cast<int>(avaibleAttackDirections.size()) - 1);
	return avaibleAttackDirections[distr(gen)];
}


int Bot::RandomAttackPlayer(Player& player) {
	auto  [ row, col ] = this->GetRandomCoords();
	while (player.tileLogic.GetTile(row, col) == 1 || player.tileLogic.GetTile(row, col) == 3) {
		tie(row, col) = this->GetRandomCoords();
	}

	auto attackResult = this->AttackPlayerTile(player, row, col);
	//bool isDestroyed = player.tileLogic.IsShipDestroyed(row, col).first;

	if (attackResult == 3 && !player.tileLogic.IsShipDestroyed(row, col).first) {
		this->hittingShip = true;
		avaibleAttackDirections = attackDirections;
		hittedShipCoords.push_back({ row, col });
		//AttackPlayer(player);
	}

	return attackResult;
}

int Bot::AttackPlayer(Player& player) {
	if (hittingShip) {
		auto [rowDir, colDir]   = this->GetRandomDirection();
		auto& currentCoords = hittedShipCoords.back();
		int newRow = currentCoords.first + rowDir;
		int newCol = currentCoords.second + colDir;
		while (newRow < 0 || newRow >= TileLogic::GetMapSize() || newCol < 0 || newCol >= TileLogic::GetMapSize()) {
			tie(rowDir, colDir) = this->GetRandomCoords();
			newRow = currentCoords.first + rowDir;
			newCol = currentCoords.second + colDir;
		}
		
		int  attackResult = this->AttackPlayerTile(player, newRow, newCol);
		bool isDestroyed  = player.tileLogic.IsShipDestroyed(newRow, newCol).first;

		if (attackResult == 3 && !isDestroyed) {
			avaibleAttackDirections.clear();
			avaibleAttackDirections = { {rowDir,colDir} };
			hittedShipCoords.push_back({ newRow, newCol });
			//AttackPlayer(player);
			return attackResult;
		}
		else if (attackResult != 3 && avaibleAttackDirections.size() == 1) {
			avaibleAttackDirections.begin()->first  = -avaibleAttackDirections.begin()->first;
			avaibleAttackDirections.begin()->second = -avaibleAttackDirections.begin()->second;
			
			pair<int, int> tempCoords = hittedShipCoords.front();
			hittedShipCoords.clear();
			hittedShipCoords.push_back(tempCoords);
			//AttackPlayer(player);
			return attackResult;
		}
		else if (attackResult != 3) {
			erase_if(avaibleAttackDirections, [rowDir, colDir](const pair<int, int>& dir) {
				return dir.first == rowDir && dir.second == colDir;
			});
			return attackResult;
		}
		avaibleAttackDirections.clear();
		hittedShipCoords.clear();
		hittingShip = false;

		return attackResult;
	}
	//avaibleAttackDirections.clear();
	//hittedShipCoords.clear();
	return -3;
}

int Bot::BotAttack(Player& player) {
	int attackResult;
	if (this->hittingShip) {
		attackResult = this->AttackPlayer(player);
	}
	else { 
		attackResult = this->RandomAttackPlayer(player);
	}
	return attackResult;
}


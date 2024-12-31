#include "Bot.h"

Bot::Bot() : rdTileDistr(0, TileLogic::GetMapSize() - 1), gen(random_device{}()), hittingShip(false) {
	attackDirections = { {0, 1}, {1, 0}, {-1, 0}, {0, -1} };
	avaibleAttackDirections = {};
}


pair<int, int> Bot::GetRandomCoords() {
	return { rdTileDistr(gen), rdTileDistr(gen) };
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
	while (player.IsAttackedTile(row, col)) {
		tie(row, col) = this->GetRandomCoords();
	}

	auto attackResult = this->AttackPlayerTile(player, row, col);

	if (attackResult == 3 && !player.IsShipDestroyed(row, col)) {
		this->hittingShip = true;
		avaibleAttackDirections = attackDirections;
		hittedShipCoords.push_back({ row, col });
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
		bool isDestroyed  = player.IsShipDestroyed(newRow, newCol);

		if (attackResult == 3 && !isDestroyed) {
			avaibleAttackDirections.clear();
			avaibleAttackDirections = { {rowDir,colDir} };
			hittedShipCoords.push_back({ newRow, newCol });
			return attackResult;
		}
		else if (attackResult != 3 && avaibleAttackDirections.size() == 1) {
			avaibleAttackDirections.begin()->first  = -avaibleAttackDirections.begin()->first;
			avaibleAttackDirections.begin()->second = -avaibleAttackDirections.begin()->second;
			
			pair<int, int> tempCoords = hittedShipCoords.front();
			hittedShipCoords.clear();
			hittedShipCoords.push_back(tempCoords);
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


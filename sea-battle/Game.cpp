#include "Game.h"

Game::Game() : player(), playerBot(), statsFilePath(""), canReceiveStats(true) {}

bool Game::InitGame(SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName) {
	player.SetTurn(true);
	return 
		player.InitPlayer(renderer, surface, cellSize, fileName) &&
		playerBot.InitPlayer(renderer, surface, cellSize, fileName);
}

bool Game::LoadMarkingTileSheet(SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName) {
	return markingTileSheet.LoadTileSheet(renderer, surface, cellSize, fileName);
}

bool Game::LoadGridTileSheet(SDL_Renderer* renderer, SDL_Surface*& surface, int cellSize, const char* fileName) {
	return gridTileSheet.LoadTileSheet(renderer, surface, cellSize, fileName);
}

bool Game::LoadStatistic(const string& filePath, string& playerName) {
	this->statsFilePath = filePath;

	string botName = "BOT";
	playerBot.LoadStatistics(this->statsFilePath, botName);

	player.LoadStatistics(this->statsFilePath, playerName);

	return true;
}

void Game::RandomizeShipLayout(bool isPlayer) {
	auto& player = isPlayer ? this->player : this->playerBot;
	player.RandomizeShipLayout();
}

void Game::RenderMarkingTile(SDL_Renderer* renderer, int index, float offsetX, float offsetY, float finalCellSize) {
	auto tileRect = this->markingTileSheet.GetTileRect(index);
	if (tileRect.w <= 0 || tileRect.h <= 0) {
		cerr << "Invalid texture rectangle for marking tile: " << index << endl;
		return;
	}

	SDL_FRect dstRect = {
		offsetX,
		offsetY,
		finalCellSize,
		finalCellSize
	};

	if (!SDL_RenderTexture(renderer, this->markingTileSheet.GetTileSheet(), &tileRect, &dstRect)) {
		cerr << "SDL_RenderTexture occured: " << SDL_GetError() << endl;
		return;
	}
}

void Game::RenderPlayer(Player& player, SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize) {
	if (player.IsTurn()) {
		player.Render(renderer, offsetX, offsetY, finalCellSize);
	}
	else {
		player.RenderAttacked(renderer, offsetX, offsetY, finalCellSize);
	}
}

void Game::RenderMarking(SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize) {
	if (!renderer) {
		cerr << "Invalid renderer for tile sheet" << endl;
		return;
	}

	for (int i = 0; i < this->markingTileSheet.GetCellsCount(); i++) {
		if (i < 10) {
			RenderMarkingTile(
				renderer, 
				i, 
				offsetX + finalCellSize + finalCellSize * i,
				offsetY, 
				finalCellSize
			);
		}
		else if (i < 20) {
			RenderMarkingTile(
				renderer, 
				i, 
				offsetX, 
				offsetY + finalCellSize + finalCellSize * (i%10),
				finalCellSize
			);
		}
	}
}

void Game::RenderGrid(SDL_Renderer* renderer, float offsetX, float offsetY, float finalCellSize) {
	if (!renderer) {
		cerr << "Invalid renderer for grid" << endl;
		return;
	}

	auto tileRect = this->gridTileSheet.GetTileRect(0);
	if (tileRect.w <= 0 || tileRect.h <= 0) {
		cerr << "Invalid texture rectangle for grid tile: " << endl;
		return;
	}

	SDL_FRect dstRect = {
		offsetX,
		offsetY,
		finalCellSize,
		finalCellSize
	};

	if (!SDL_RenderTexture(renderer, this->gridTileSheet.GetTileSheet(), &tileRect, &dstRect)) {
		cerr << "SDL_RenderTexture occured: " << SDL_GetError() << endl;
		return;
	}
}

void Game::RenderText(SDL_Renderer* renderer, TTF_Font* font, const string& text, SDL_Color color, float offsetX, float offsetY) {
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), text.length(), color);
	if (!textSurface) {
		cerr << "Failed to create text surface: " << SDL_GetError() << endl;
		return;
	}
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (!textTexture) {
		cerr << "Failed to create text texture: " << SDL_GetError() << endl;
		SDL_DestroySurface(textSurface);
		return;
	}
	SDL_FRect    textRect    = { offsetX - static_cast<float>(textSurface->w)/2.f, offsetY, static_cast<float>(textSurface->w), static_cast<float>(textSurface->h)};

	SDL_RenderTexture(renderer, textTexture, NULL, &textRect);
	SDL_DestroyTexture(textTexture);
	SDL_DestroySurface(textSurface);
}


void Game::Render(SDL_Renderer* renderer, bool isPlayer, float offsetX, float offsetY, float finalCellSize) {
	if (!renderer) {
		cerr << "Invalid renderer for game" << endl;
		return;
	}

	auto& player = isPlayer ? this->player : this->playerBot;
	float finalOffsetX = isPlayer ? offsetX : offsetX + finalCellSize * 12.f;

	RenderPlayer(player, renderer, finalOffsetX, offsetY, finalCellSize);
	RenderMarking(renderer, finalOffsetX - finalCellSize, offsetY - finalCellSize, finalCellSize     );
	RenderGrid(   renderer, finalOffsetX - finalCellSize, offsetY - finalCellSize, finalCellSize * 11);	
		
	string fontPath = SDL_GetBasePath() + string("fonts/font.ttf");
	TTF_Font* font = TTF_OpenFont(fontPath.c_str(), finalCellSize);
	if (font) {
		SDL_Color color = { 255, 255, 255 };
		if (isPlayer) {
			string text = this->player.GetName();
			RenderText(
				renderer,
				font,
				text,
				color,
				offsetX + finalCellSize * 5.f,
				offsetY + finalCellSize * 9.9f
			);
			int wins = this->player.GetWinCount();
			int losses = this->player.GetLoseCount();
			RenderText(
				renderer,
				font,
				"Wins: " + to_string(wins) + " Loses: " + to_string(losses),
				color,
				offsetX + finalCellSize * 5.f,
				offsetY - finalCellSize * 2.1f
			);
		}
		else {
			string text = this->playerBot.GetName();
			RenderText(
				renderer,
				font,
				text,
				color,
				offsetX + finalCellSize * 17.f,
				offsetY + finalCellSize * 9.9f
			);
			int wins = this->playerBot.GetWinCount();
			int losses = this->playerBot.GetLoseCount();
			RenderText(
				renderer,
				font,
				"Wins: " + to_string(wins) + " Loses: " + to_string(losses),
				color,
				offsetX + finalCellSize * 17.f,
				offsetY - finalCellSize * 2.1f
			);
		}


	}
}

bool Game::AttackBotTile(int row, int column) {
	//auto& player   = IsPlayerTurn() ? this->player : this->playerBot;
	//auto& opponent = IsPlayerTurn() ? this->playerBot : this->player;
	if (this->player.IsTurn()) {
		auto result = this->player.AttackPlayerTile(this->playerBot, row, column);
		if (result == 3 || result == -2 || result == -1) {
			return true;
		}
	}

	SwitchTurn();
	return false;
}

bool Game::BotAttackCycle() {
	if (IsWon()) {
		return false;
	}
	if (this->playerBot.IsTurn()) {
		int result = this->playerBot.BotAttack(this->player);
		if (result == 3 || result == -2 || result == -1) {
			return BotAttackCycle();
		}

		SwitchTurn();
		return false;
	}
	return false;
}

void Game::SwitchTurn() {
	player.SetTurn(!player.IsTurn());
	playerBot.SetTurn(!playerBot.IsTurn());
}

const bool Game::IsPlayerTurn() const {
	return player.IsTurn();
}

const int Game::IsWon() {
	if (player.HasLost()) {
		if (this->canReceiveStats) {
			this->playerBot.IncreaseWinCount();
			this->player.IncreaseLoseCount();
			SaveStats();
			this->canReceiveStats = false;
		}
		return 2;
	}
	if (playerBot.HasLost()) {
		if (this->canReceiveStats) {
			this->player.IncreaseWinCount();
			this->playerBot.IncreaseLoseCount();
			SaveStats();
			this->canReceiveStats = false;
		}
		return 1;
	}
	return 0;
}

void Game::SaveStats() {
	this->player.SaveStatistics(this->statsFilePath);
	this->playerBot.SaveStatistics(this->statsFilePath);
}

void Game::RestartGame() {
	RandomizeShipLayout(true);
	RandomizeShipLayout(false);
	this->player.SetTurn(true);
	this->playerBot.SetTurn(false);
	this->canReceiveStats = true;
}

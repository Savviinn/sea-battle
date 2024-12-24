#include <SDL3/SDL_ttf.h>
#include <SDL3/SDL_main.h>

#include "Game.h"
#include "Options.h"

using namespace std;

int gameWindowWidth;
int gameWindowHeight;
const char* gameName = "Sea battle";

string basePath		  = SDL_GetBasePath();
string optionsName	  = "OPTIONS.ini";
string statisticsName = "statistics.ini";

SDL_Window*   gameWindow;
SDL_Renderer* renderer;
SDL_Surface*  tileSheetSurface;
SDL_Texture*  tileSheet;

void QuitGame() {
	SDL_DestroySurface(tileSheetSurface);
	SDL_DestroyTexture(tileSheet);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(gameWindow);
	IMG_Quit();
	SDL_Quit();
}

int SDL_main(int argc, char* argv[])
{
	if (!LoadOptions(basePath + optionsName, gameWindow, gameWindowWidth, gameWindowHeight, gameName)) {
		return -1;
	}

	if (!TTF_Init()) {
		cerr << "Failed to initialize TTF: " << SDL_GetError() << endl;
		TTF_Quit();
		SDL_Quit();
		return -1;
	}

	renderer = SDL_CreateRenderer(gameWindow, NULL);
	if (!renderer) {
		SDL_Quit();
		return -1;
	}

	SDL_FRect gameWindowRect = { 0.f, 0.f, static_cast<float>(gameWindowWidth), static_cast<float>(gameWindowHeight) };
	const float finalCellSize = min(
		static_cast<float>(gameWindowWidth / 25.f),
		static_cast<float>(gameWindowHeight / 13.f)
	);
	const float gapSizeX = (gameWindowWidth - finalCellSize * 25.f) / 2.f + finalCellSize * 2;
	const float gapSizeY = (gameWindowHeight - finalCellSize * 13.f) / 2.f + finalCellSize * 2;

	string fontPath = basePath + string("fonts/font.ttf");
	TTF_Font* font = TTF_OpenFont(fontPath.c_str(), finalCellSize);
	float smallerFontSize = finalCellSize / 1.75f;
	TTF_Font* smallerFont = TTF_OpenFont(fontPath.c_str(), smallerFontSize);
	if (!font || !smallerFont) {
		cerr << "Failed to open font: " << SDL_GetError() << endl;
	}
	SDL_Color colorWhite = { 255, 255, 255 };
	SDL_Color colorBlack = { 0, 0, 0 };

	Game game;

	SDL_Event e;
	bool needsRedraw = true;
	bool entersName = true;
	bool running = true;

	string playerName = "";
	while (running && entersName) {
		if (needsRedraw) {
			string namePrompt1 = "Enter Your name: " + playerName;
			string namePrompt2 = "Press \"Return\" to confirm";

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &gameWindowRect);
			SDL_RenderRect(renderer, &gameWindowRect);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			game.RenderText(renderer, smallerFont, namePrompt1, colorBlack, gameWindowWidth / 2.f, gameWindowHeight / 2.f - finalCellSize * 0.5f - smallerFontSize);
			game.RenderText(renderer, smallerFont, namePrompt2, colorBlack, gameWindowWidth / 2.f, gameWindowHeight / 2.f - finalCellSize * 0.5f + smallerFontSize);
			
			SDL_RenderPresent(renderer);
			needsRedraw = false;
		}

		SDL_WaitEvent(&e);
		if (e.type == SDL_EVENT_QUIT) {
			running = false;
			QuitGame();
			return 0;
			break;
		}
		else if (e.type == SDL_EVENT_KEY_DOWN) {
			switch (e.key.scancode) {
			case SDL_SCANCODE_ESCAPE:
				running = false;
				QuitGame();
				return 0;
				break;
			case SDL_SCANCODE_BACKSPACE:
				if (!playerName.empty()) {
					playerName.pop_back();
					needsRedraw = true;
				}
				break;
			case SDL_SCANCODE_RETURN:
				if (!playerName.empty() && playerName != "bot") {
					entersName = false;
				}
				break;
			default:
				if (e.key.key >= SDLK_A && e.key.key <= SDLK_Z || e.key.key >= SDLK_0 && e.key.key <= SDLK_9) {
					playerName += static_cast<char>(e.key.key);
					needsRedraw = true;
				}
				break;
			}
		}
	}

	game.InitGame(renderer, tileSheetSurface, 32, "map.png");
	game.LoadStatistic(basePath + statisticsName, playerName);
	game.LoadMarkingTileSheet(renderer, tileSheetSurface, 16, "marking.png");
	game.LoadGridTileSheet(renderer, tileSheetSurface, 176, "grid.png");

	string cursorPath = basePath + "tiles/cursor.png";
	tileSheetSurface = IMG_Load(cursorPath.c_str());
	if (!tileSheetSurface) {
		cerr << "Failed to load cursor image: " << SDL_GetError() << endl;
	}
	tileSheet = SDL_CreateTextureFromSurface(renderer, tileSheetSurface);
	if (!tileSheet) {
		cerr << "Failed to create cursor texture: " << SDL_GetError() << endl;
	}
	SDL_DestroySurface(tileSheetSurface);

	SDL_Point cursorPos = { 0, 0 };
	SDL_FRect cursorRect = {};


	game.RandomizeShipLayout(true);
	game.RandomizeShipLayout(false);


	needsRedraw = true;
	bool randomizeConfirm = false;

	while (running) {
		if (needsRedraw) {
			game.BotAttackCycle();

			SDL_SetRenderDrawColor(renderer, 0, 162, 232, 255);
			SDL_RenderFillRect(renderer, &gameWindowRect);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			game.Render(renderer, true, gapSizeX, gapSizeY, finalCellSize);

			if (!randomizeConfirm) {
				string textConfirm1 = "Press \"Enter\" to confirm ship layout";
				string textConfirm2 = "Press \"R\" to randomize ship layout";
				game.RenderText(renderer, smallerFont, textConfirm1, colorWhite, gapSizeX + finalCellSize * 16, gameWindowHeight / 2.f - finalCellSize * 0.5f - smallerFontSize);
				game.RenderText(renderer, smallerFont, textConfirm2, colorWhite, gapSizeX + finalCellSize * 16, gameWindowHeight / 2.f - finalCellSize * 0.5f + smallerFontSize);
			}
			else {
				game.Render(renderer, false, gapSizeX, gapSizeY, finalCellSize);

				cursorRect = {
					gapSizeX + finalCellSize * 12.f + cursorPos.x * finalCellSize,
					gapSizeY + cursorPos.y * finalCellSize,
					finalCellSize,
					finalCellSize
				};

				SDL_RenderTexture(renderer, tileSheet, nullptr, &cursorRect);
			}

			SDL_RenderPresent(renderer);
			needsRedraw = false;
		}

		if (!randomizeConfirm) {
			SDL_WaitEvent(&e);
			if (e.type == SDL_EVENT_QUIT) {
				running = false;
			}
			else if (e.type == SDL_EVENT_KEY_DOWN) {
				switch (e.key.scancode) {
				case SDL_SCANCODE_ESCAPE:
					running = false;
					break;
				case SDL_SCANCODE_R:
					game.RandomizeShipLayout(true);
					needsRedraw = true;
					break;
				case SDL_SCANCODE_RETURN:
					randomizeConfirm = true;
					needsRedraw = true;
					break;
				}
			}
		}
		else if (int GameResult = game.IsWon()) {

			SDL_SetRenderDrawColor(renderer, 0, 162, 232, 255);
			SDL_RenderFillRect(renderer, &gameWindowRect);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			string gameResultText = GameResult == 1 ? "CONGRATULATIONS! YOU WON!" : "YOU LOST!";
			string gamePostResultText1 = "Press \"Escape\" to quit the game";
			string gamePostResultText2 = "Or press \"R\" to restart";

			SDL_Color resultColor{};
			if (GameResult == 2) {
				resultColor = { 215, 32, 64 };
			}
			else {
				resultColor = { 32, 255, 64 };
			}

			game.RenderText(renderer, font, gameResultText, resultColor, gameWindowWidth / 2.f, gameWindowHeight / 2.f - finalCellSize * 2.f);
			game.RenderText(renderer, font, gamePostResultText1, colorWhite, gameWindowWidth / 2.f, gameWindowHeight / 2.f - finalCellSize * 0.5f);
			game.RenderText(renderer, font, gamePostResultText2, colorWhite, gameWindowWidth / 2.f, gameWindowHeight / 2.f + finalCellSize * 1.f);

			SDL_RenderPresent(renderer);

			SDL_WaitEvent(&e);
			if (e.type == SDL_EVENT_QUIT) {
				running = false;
				QuitGame();
				return 0;
			}
			else if (e.type == SDL_EVENT_KEY_DOWN) {
				switch (e.key.scancode) {
				case SDL_SCANCODE_ESCAPE:
					running = false;
					QuitGame();
					return 0;
					break;
				case SDL_SCANCODE_R:
					game.RestartGame();
					needsRedraw = true;
				}
			}
		}
		else {
			SDL_WaitEvent(&e);
			if (e.type == SDL_EVENT_QUIT) {
				running = false;
			}
			else if (e.type == SDL_EVENT_KEY_DOWN) {
				switch (e.key.scancode) {
				case SDL_SCANCODE_ESCAPE:
					running = false;
					break;
				case SDL_SCANCODE_RETURN:
					game.AttackBotTile(cursorPos.y, cursorPos.x);
					needsRedraw = true;
					break;
				case SDL_SCANCODE_UP:
					if (cursorPos.y > 0) {
						--cursorPos.y;
						needsRedraw = true;
					}
					break;
				case SDL_SCANCODE_DOWN:
					if (cursorPos.y < 9) {
						++cursorPos.y;
						needsRedraw = true;
					}
					break;
				case SDL_SCANCODE_LEFT:
					if (cursorPos.x > 0) {
						--cursorPos.x;
						needsRedraw = true;
					}
					break;
				case SDL_SCANCODE_RIGHT:
					if (cursorPos.x < 9) {
						++cursorPos.x;
						needsRedraw = true;
					}
					break;
				default:
					break;
				}
			}
		}
	}

	QuitGame();
	return 0;
}

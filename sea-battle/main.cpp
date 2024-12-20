#include <SDL3/SDL_main.h>
#include <SDL3/SDL_ttf.h>

#include "Game.h"
#include "Options.h"

using namespace std;

int gameWindowWidth;
int gameWindowHeight;
const char* gameName = "Sea battle";

string basePath    = SDL_GetBasePath();
string optionsName = "OPTIONS.ini";

SDL_Window*   gameWindow;
SDL_Renderer* renderer;
SDL_Surface*  tileSheetSurface;
SDL_Texture*  tileSheet;

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

	Game game;
	game.InitGame(			  renderer, tileSheetSurface, 32,  "map.png"    );
	game.LoadMarkingTileSheet(renderer, tileSheetSurface, 16,  "marking.png");
	game.LoadGridTileSheet(	  renderer, tileSheetSurface, 176, "grid.png"   );

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

	SDL_Point cursorPos  = { 0, 0 };
	SDL_FRect cursorRect = {};

	SDL_FRect gameWindowRect = { 0.f, 0.f, static_cast<float>(gameWindowWidth), static_cast<float>(gameWindowHeight) };
	const float finalCellSize = min(
		static_cast<float>(gameWindowWidth / 25.f), 
		static_cast<float>(gameWindowHeight / 13.f)
	);
	const float gapSizeX = (gameWindowWidth  - finalCellSize * 25.f) / 2.f + finalCellSize * 2;
	const float gapSizeY = (gameWindowHeight - finalCellSize * 13.f) / 2.f + finalCellSize * 2;

	//game.RandomizeShipLayout();
	game.RandomizeShipLayout();

	SDL_Event e;
	bool running = true;
	bool needsRedraw = true;

	while (running) {
		if (needsRedraw) {
			if (game.IsWon()) {
				SDL_SetRenderDrawColor(renderer, 0, 162, 232, 255);
				SDL_RenderFillRect(renderer, &gameWindowRect);
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

				string fontPath = basePath + string("fonts/UbuntuSansMono-Regular.ttf");
				TTF_Font* font = TTF_OpenFont(fontPath.c_str(), finalCellSize);
				if (font) {
					string text = game.IsWon() == 1 ? "CONGRATULATIONS! YOU WON!" : "YOU LOST!";
					game.RenderText(renderer, finalCellSize * 6, font, text, {255, 255, 255}, gameWindowWidth/2.f, gameWindowHeight/2.f);
				}

				SDL_RenderPresent(renderer);


				SDL_Delay(3000);
				running = false;
				break;
			}

			game.BotAttackCycle();

			SDL_SetRenderDrawColor(renderer, 0, 162, 232, 255);
			SDL_RenderFillRect(renderer, &gameWindowRect);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			
			game.Render(renderer, gapSizeX, gapSizeY, finalCellSize);

			if (game.IsPlayerTurn()) {
				cursorRect = {
					gapSizeX + finalCellSize * 12.f + cursorPos.x * finalCellSize,
					gapSizeY + cursorPos.y * finalCellSize,
					finalCellSize,
					finalCellSize
				};
			}
			SDL_RenderTexture(renderer, tileSheet, nullptr, &cursorRect);

			SDL_RenderPresent(renderer);
			needsRedraw = false;
		}

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
				game.SwitchTurn();
				needsRedraw = true;
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
				cout << e.key.scancode << endl;
				break;
			}
		}
	}

	SDL_DestroySurface(tileSheetSurface);
	SDL_DestroyTexture(tileSheet);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(gameWindow);
	IMG_Quit();
	SDL_Quit();
	return 0;
}
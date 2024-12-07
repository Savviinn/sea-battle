#pragma once
#include <SDL3/SDL.h>

bool Render(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_FRect* srcrect, const SDL_FRect* dstrect) {
	return SDL_RenderTexture(renderer, texture, srcrect, dstrect);
}
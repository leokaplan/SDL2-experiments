#include <iostream>
#include <SDL2/SDL.h>
#include "globals.h"
#include "error.h"
#include "loadTexture.h"
#include "render.h"

int main(int argc, char **argv){
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("SDL window", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == nullptr){
			logSDLError(std::cout, "CreateWindow");
			return 2;
		}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == nullptr){
			logSDLError(std::cout, "CreateRenderer");
			return 3;
		}


	SDL_Texture* lena = loadTexture("./lena.bmp",renderer);
	SDL_Texture* bg = loadTexture("./bg.bmp",renderer);
	if (bg == nullptr || lena == nullptr)
		return 4;
	
	SDL_Event e;
	bool quit = false;
	int lx = 100;
	int ly = 100;
	bool l = true;
	while (!quit){
		while (SDL_PollEvent(&e)){
			l = true;
			if (e.type == SDL_QUIT)
				quit = true;
			if (e.type == SDL_KEYDOWN)
				switch (e.key.keysym.sym){
					case SDLK_1:
						lx++;
						break;
					case SDLK_2:
						ly++;
						break;
					case SDLK_3:
						lx--;
						break;
					case SDLK_4:
						ly--;
						break;
					case SDLK_5:
						lx=100;
						ly=100;
						break;
					case SDLK_ESCAPE:
						quit = true;
						break;
					default:
						break;
				}
			if (e.type == SDL_MOUSEBUTTONDOWN)
				l = false;
		}
		SDL_RenderClear(renderer);
		tileTexture(bg,renderer);
		if(l) centerTexture(lena,renderer,lx,ly);
		SDL_RenderPresent(renderer);
	}


	SDL_DestroyTexture(lena);
	SDL_DestroyTexture(bg);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();


	return 0;
}

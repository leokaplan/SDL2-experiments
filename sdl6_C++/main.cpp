#include <iostream>
#include <SDL2/SDL.h>
#include <cstdlib>
#include "globals.h"
#include "error.h"
#include "rect.cpp"

int main(int argc, char **argv){
	printf("oi");
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
	Uint32 T_start = 0; 
	bool T_running = true;
	SDL_Event e;
	bool quit = false;
	Rect* r[10000];
	int timer1 = 0;
	int timer2 = 0;

	T_start = SDL_GetTicks(); 
	
	while (!quit){
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT)
				quit = true;
			if (e.type == SDL_KEYDOWN)
				switch (e.key.keysym.sym){
					case SDLK_SPACE:
						if( T_running == true ) { 
							T_running = false; 
							T_start = 0; 
						} 
						else { 
							T_running = true; 
							T_start = SDL_GetTicks(); 
						}
					case SDLK_ESCAPE:
						quit = true;
						break;
					default:
						break;
				}
			Uint32 dt = SDL_GetTicks() - T_start;
			if( T_running == true ) { 
				if (timer1>40){
                    for(int i = 0;i<30;i++){
                        r[i] = new Rect(50 + random()%20, 50, random()%750,renderer);
                    }
                   	timer1 = 0;
                }
                for(int i = 0;i<30;i++){      // create 30 rectangles every 40ms
                    r[i]->update(dt);
                }
                if (timer2>30000){
                    while(obj)
                        delete r[obj-1];
                    
                   	timer2 = 0;
                }
                timer1 += dt;
                timer2 += dt;
			} 
		}
		SDL_RenderClear(renderer);
		//SDL_SetRenderDrawColor(ren, 0,0,0, 0xFF);
        //SDL_RenderFillRect(ren, &bg);
		for(int i = 0;i<30;i++){   
            r[i]->render();
        }
		SDL_RenderPresent(renderer);
	}


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();


	return 0;
}

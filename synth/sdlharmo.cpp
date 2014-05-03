#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <SDL2/SDL.h>

#define NCOEF	(12)

#define SCREEN_WIDTH	(800)
#define SCREEN_HEIGHT	(500)
#define SCREEN_DEPTH	(32)
#define BPM (500)
#define CLICK (100)

#include "harmo.c"

int main(void)
{
	Synth *synth;
	SDL_Event event;
		
	/* SDL initialization */
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("SDL harmo", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == nullptr){
			return 2;
		}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == nullptr){
			return 3;
		}
	/* Synth init */
	
	synth = make_synth();
	
	printf("Synth: %f Hz %f s\n", synth->fs, synth->Ts);
	
	open_audio(synth);
	SDL_PauseAudio(0);
	
	/* Main loop */
	
	int quit = 0;
	Uint32 T_start = 0;
	int timer = 0;
	int timer2 = 0;
	int note = 0;
	int music[] = {0,0,1,2,0,-2,-1,-2,-2,0,2,2,0,-2,0};
	int music_tam = sizeof(music)/sizeof(int) - 1;
	while(!quit)
	{
		/* Events */
		while ( SDL_PollEvent(&event) )
		{
			if (event.type == SDL_QUIT)
			{
				printf("Exiting...\n");
				quit = 1;
			}
			else if(event.type == SDL_MOUSEBUTTONDOWN)
			{
				treat_click(event.button.x, event.button.y, synth);
			}
			else if(event.type == SDL_MOUSEMOTION && event.motion.state != 0)
			{
				treat_click(event.motion.x, event.motion.y, synth);
			}
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_1){
				change_note(synth,1);
			}
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_2){
				change_note(synth,-1);
			}
				
		}
		
		/* Play music */
		if(timer2>CLICK){
			SDL_PauseAudio(0);
			timer2 = 0;
		}
		if(timer>BPM){
			SDL_PauseAudio(1);
			if(note>music_tam) {
				note = 0;
				reset_note(synth);
			}
			change_note(synth,music[note]);
			note = note + 1;
			timer = 0;
		}

		Uint32 dt = SDL_GetTicks() - T_start;
		timer2+=dt;
		timer += dt;
		T_start += dt;


		/* Drawing */
		SDL_Rect bg;
		bg.w = SCREEN_WIDTH;
    	bg.h = SCREEN_HEIGHT;
    	bg.x = 0;
    	bg.y = 0;
		SDL_SetRenderDrawColor(renderer, 0,0,0, 0xFF);
        SDL_RenderFillRect(renderer, &bg);
		
		for (int i = 0; i < NCOEF ; i++)
		{
			SDL_Rect rect;
			
			rect.x = i * SCREEN_WIDTH / NCOEF;
			rect.w = SCREEN_WIDTH / NCOEF;
			
			rect.y = (1.0 - synth->coefs[i]) * SCREEN_HEIGHT;
			rect.h = synth->coefs[i] * SCREEN_HEIGHT;	
			SDL_SetRenderDrawColor(renderer, 255,0,0, 0xFF);
            SDL_RenderFillRect(renderer, &rect);
		}
			
		

	SDL_RenderPresent(renderer);
	}


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	free(synth);
	return 0;
}

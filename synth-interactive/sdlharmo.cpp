#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>
#include <string>
using namespace std;

#define NCOEF	(12)

#define SCREEN_WIDTH	(800)
#define SCREEN_HEIGHT	(500)
#define SCREEN_DEPTH	(32)


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
	//TTF_Init();
	/* Synth init */
	
	synth = make_synth();
	
	printf("Synth: %f Hz %f s\n", synth->fs, synth->Ts);
	
	open_audio(synth);
	SDL_PauseAudio(1);
	
	/* Main loop */
	
	int quit = 0;
	Uint32 T_start = 0;
	int timer = 0;
	int timer2 = 0;
	int note = 0;
	vector<int> music;
	int music_tam = 0;
	int BPM  = 500;
	int CLICK = BPM;
	int new_note = 0;
	int font_h = 10;
	int keys[] = {SDLK_r,SDLK_t,SDLK_y,SDLK_u,SDLK_i,SDLK_o,SDLK_p}; 
	int scale[] = {0,3,5,6,7,10,12};
	int keys_tam = 7;
	int key_block = false;
	int pressed;
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
			else if (event.type == SDL_KEYDOWN){
				if(event.key.keysym.sym == SDLK_1){
					reset_note(synth);
					change_note(synth,1);
					note = 0;
				}
				else if (event.key.keysym.sym == SDLK_2){
					reset_note(synth);
					change_note(synth,-1);
					note = 0;
				}
				else if (event.key.keysym.sym == SDLK_3){
					new_note++;
				}
				else if (event.key.keysym.sym == SDLK_4){
					new_note--;
				}
				else if (event.key.keysym.sym == SDLK_5){
					music.push_back(new_note);
					music_tam++;
					new_note = 0;
				}
				else if (event.key.keysym.sym == SDLK_6){
					music.erase(music.begin(), music.end());
					music_tam = 0;
					new_note = 0;
				}
				else if (event.key.keysym.sym == SDLK_q){
					CLICK++;
				}
				else if (event.key.keysym.sym == SDLK_a){
					CLICK--;
				}
				else if (event.key.keysym.sym == SDLK_z){
					CLICK = 0;
				}
				else if (event.key.keysym.sym == SDLK_x){
					CLICK = BPM;
				}
				else if (event.key.keysym.sym == SDLK_w){
					BPM++;
				}
				else if (event.key.keysym.sym == SDLK_s){
					BPM--;
				}
				/* interactive mode */

				if(!key_block){
					for(int j = 0;j<keys_tam;j++){
						if(event.key.keysym.sym == keys[j]){
							//printf("%d\n",j);
							SDL_PauseAudio(0);
							change_note(synth,scale[j]);
							key_block = true;
							pressed = j;
							break;
						}
					}
				}
			}
			else if (event.type == SDL_KEYUP){
				if(key_block){
						if(event.key.keysym.sym == keys[pressed]){
							key_block = false;
							reset_note(synth);
							SDL_PauseAudio(1);
						}
				}
			}
		}


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
			rect.h = synth->coefs[i] * SCREEN_HEIGHT - 3*font_h;	
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

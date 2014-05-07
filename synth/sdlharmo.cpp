#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"

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
	TTF_Init();
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
	vector<int> music;
	int music_tam = 0;
	int BPM  = 500;
	int CLICK = BPM;
	int new_note = 0;
	int font_h = 12;
	SDL_Texture* TextTexture;
 	TTF_Font * gFont = TTF_OpenFont( "monofont.ttf", font_h );
 	SDL_Color textColor = { 0, 255, 0 };

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
			}
		}
		
		/* Play music */
		if(timer2>CLICK){
			SDL_PauseAudio(1);
			timer2 = 0;
		}
		if(timer>BPM){
			if(note>=music_tam) {
				note = 0;
				reset_note(synth);
			}
			if(music_tam) {
				SDL_PauseAudio(0);
				change_note(synth,music[note]);  
			}
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

		string textureText = "";
		textureText.append("BPM:");
		textureText.append(to_string(BPM));
		textureText.append(" CLICK:");
		textureText.append(to_string(CLICK));
		textureText.append(" SEQ:");
		for (int i = 0; i < music_tam; i++)
		{
			if(music[i]>=0) textureText.push_back(music[i] +'0');
			else {
				textureText.push_back('-');
				textureText.push_back(-1*music[i] +'0');
			}
		}
		if(new_note>=0) textureText.push_back(new_note +'0');
		else {
			textureText.push_back('-');
			textureText.push_back(-1*new_note +'0');
		}
		/* Text */
		SDL_Rect str_r;
    	str_r.x = 0;
    	str_r.y = 0;
    	str_r.w = font_h*textureText.size()*1.5;
    	str_r.h = font_h*1.7;
		SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
		TextTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
		SDL_RenderCopy(renderer, TextTexture, 0, &str_r);
		SDL_FreeSurface( textSurface );
		SDL_RenderPresent(renderer);
	}

 	free(TextTexture);
 	TTF_CloseFont(gFont); 
 	gFont = NULL;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	free(synth);
	return 0;
}

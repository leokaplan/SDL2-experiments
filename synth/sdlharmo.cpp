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

vector<string> notes = {"C","C#","D","Eb","E","F","F#","G","Ab","A","Bb","B"};

int get_note(int interval){
	int notes_tam = notes.size() - 1;
	int real_note = get_root() + interval;
	if(real_note >= notes_tam + 1 ) return real_note%notes_tam;
	else if(real_note < 0 ) return notes_tam + real_note%notes_tam; 
	return real_note;
}
//TODO:
//int get_octave(int interval);

int main(void)
{
	/* instructions */
	
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

	printf("--------------instructions------------\n");
	printf("mouse to change the instrument\n1 to increase root note by 1 semitone\n2 to decrease root note by 1 semitone\n3 and 4 to change new note by 1 semitone\n");
	printf("5 add a note playing\n6 clean music\n7 add a note not playing\n8 play music bufferized by 7\n");
	printf("q up click\na down click\nz zero click\nx reset click\nw up BPM\ns down BPM\n");
	printf("--------------------------------------\n");

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
					new_note--;
				}
				else if (event.key.keysym.sym == SDLK_4){
					new_note++;
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
				else if (event.key.keysym.sym == SDLK_7){
					music.push_back(new_note);
					new_note = 0;
				}
				else if (event.key.keysym.sym == SDLK_8){
					music_tam += music.size()-music_tam;
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
				reset_note(synth);
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
			textureText.append(notes[get_note(music[i])]);	
			//textureText.append(notes[get_octave(music[i])]);			
		
		}
		textureText.append(notes[get_note(new_note)]);
		//textureText.append(notes[get_octave(new_note)]);

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

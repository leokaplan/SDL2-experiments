#ifndef RENDER_H
#define RENDER_H 

#include <SDL2/SDL.h>
#include "globals.h"
#include "error.h"


void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,int w,int h);
void tileTexture(SDL_Texture *tex, SDL_Renderer *ren);
void tileTexture(SDL_Texture *tex, SDL_Renderer *ren,int tile_size_x,int tile_size_y);
void centerTexture(SDL_Texture *tex, SDL_Renderer *ren);
void centerTexture(SDL_Texture *tex, SDL_Renderer *ren,int w,int h);

#endif

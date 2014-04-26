#include "render.h"

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Query the texture to get its width and height to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,int w,int h){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

void tileTexture(SDL_Texture *tex, SDL_Renderer *ren,int tile_size_x,int tile_size_y){
	int w,h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	int n_tiles_x = SCREEN_WIDTH / w;
	int n_tiles_y = SCREEN_HEIGHT / h;
	for (int i = 0; i < n_tiles_x * n_tiles_y; i++){
		int x = i % n_tiles_x;
		int y = i / n_tiles_x;
		renderTexture(tex, ren,x*w,y*h);
	}
}
void tileTexture(SDL_Texture *tex, SDL_Renderer *ren){
	int n_tiles_x = SCREEN_WIDTH / TILE_SIZE_X;
	int n_tiles_y = SCREEN_HEIGHT / TILE_SIZE_Y;
	for (int i = 0; i < n_tiles_x * n_tiles_y; i++){
		int x = i % n_tiles_x;
		int y = i / n_tiles_x;
		renderTexture(tex, ren,x*TILE_SIZE_X,y*TILE_SIZE_Y);
	}
}
void centerTexture(SDL_Texture *tex, SDL_Renderer *ren){
	int w,h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	int x = SCREEN_WIDTH / 2 - w / 2;
	int y = SCREEN_HEIGHT / 2 - h / 2;
	renderTexture(tex, ren, x, y);
}
void centerTexture(SDL_Texture *tex, SDL_Renderer *ren,int w,int h){
	int x = SCREEN_WIDTH / 2 - w / 2;
	int y = SCREEN_HEIGHT / 2 - h / 2;
	renderTexture(tex, ren, x, y,w,h);
}
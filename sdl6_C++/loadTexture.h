#ifndef TEXTURE_H
#define TEXTURE_H 

#include <SDL2/SDL.h>
#include "error.h"

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

#endif
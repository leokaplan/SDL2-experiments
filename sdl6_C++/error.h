#ifndef ERROR_H
#define ERROR_H 

#include <iostream>
#include <SDL2/SDL.h>

void logSDLError(std::ostream &os, const std::string &msg);

#endif
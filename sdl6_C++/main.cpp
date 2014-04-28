#include <iostream>
#include <list>
#include <SDL2/SDL.h>
#include <cstdlib>
#include "globals.h"
#include "error.h"
#include "rect.cpp"
using namespace std;

int obj = 0;
list<Rect> r;


int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL window", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        logSDLError(std::cout, "CreateWindow");
        return 2;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        logSDLError(std::cout, "CreateRenderer");
        return 3;
    }
    Uint32 T_start = 0;
    bool T_running = true;
    SDL_Event e;
    bool quit = false;
    list<Rect>::iterator it;
    int timer1 = 0;
    int timer2 = 0;
    SDL_Rect bg;
    bg.w = SCREEN_WIDTH;
    bg.h = SCREEN_HEIGHT;
    bg.x = 0;
    bg.y = 0;

    T_start = SDL_GetTicks();

    while (!quit) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
            if (e.type == SDL_KEYDOWN)
                switch (e.key.keysym.sym) {
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
        }
        Uint32 dt = SDL_GetTicks() - T_start;
        if( T_running == true ) {
            if (timer1>40) {
                for(int i = 0; i<30; i++) {
                    if(obj<max_obj)
                        r.push_back(Rect(500 + (random()%200), 50, random()%750,renderer));
                }
                timer1 = 0;
            }
            if (timer2>30000) {
                r.erase(r.begin(), r.end());
                timer2 = 0;
            }
            SDL_SetRenderDrawColor(renderer, 0,0,0, 0xFF);
            SDL_RenderFillRect(renderer, &bg);
            for (it = r.begin(); it != r.end(); it++) {
                (*it).update(dt);
                (*it).render();
                if ((*it).died())
                    it = r.erase(it);
            }
            SDL_RenderPresent(renderer);
            T_start +=dt;
            timer1 += dt;
            timer2 += dt;
        }
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}

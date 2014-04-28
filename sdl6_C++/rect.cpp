#include <SDL2/SDL.h>
#include "error.h"
#include "globals.h"
#include <cstdio>
class Rect{
    SDL_Rect  rct;        
    int vel;         
    SDL_Renderer* ren;
   	SDL_Color clr;  

    public:
    Rect(int vel,int x,int y,SDL_Renderer* ren){
    	this->vel = vel;
        this->rct.w = 10;
    	this->rct.h = 10;
    	this->rct.x = x;
        this->rct.y = y;
        this->clr.r = 0xFF;
        this->clr.g = 0xFF;
        this->clr.b = 0xFF;
        this->ren = ren;
        obj++;
    }
    ~Rect(){
        obj--;
        //this = NULL; 
    }
    void say(){
        printf("say: %d %d\n",this->vel,this->rct.x);
    }
    void render(){
        SDL_SetRenderDrawColor(ren, clr.r,clr.g,clr.b,0xFF);
        SDL_RenderFillRect(ren, &rct);
    }

    void update(int dt){
    	static int T;
    	if(T>200){
    		rct.h = rct.h + random()%5 - 2;     // random height
            if(rct.h <= 0)
                rct.h = 1;
            

            rct.w = rct.w + random()%5 - 2;     // random width
            if (rct.w <= 0)
                rct.w = 1;
            

            clr.r = clr.r + random()%5 - 2;          // random r/g/b
            clr.g = clr.g + random()%5 - 2;
            clr.b = clr.b + random()%5 - 2;

            T=0;
        }
        T += dt;
        float dx = (vel*dt)/1000;
        rct.x = rct.x + dx;
    }
    int died(){
        if (rct.x > 300) return true;
        return false;          
    }
};
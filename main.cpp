#include <iostream>
#include "headers/SIR.h"
#include "headers/graph.h"
#include "headers/menu.h"
#ifdef _WIN32
  #include "src/win/SDL2/SDL.h"
  #include "src/win/SDL2/SDL_ttf.h"
  int WIDTH = 1100, HEIGHT = 900;
#else
  #include <SDL.h>
  #include <SDL_ttf.h>
  int WIDTH = 800, HEIGHT = 600;
#endif

bool running = true;

int main(int argc, char *argv[])
{   
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    SDL_Window * window = SDL_CreateWindow("MD", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    
    TTF_Font* font = TTF_OpenFont("./fonts/prototype.ttf", 20);

    int scene = 0;

    while(running)
    {   
        switch(scene){
            case 0:
            menu(&scene, renderer, font);
            break;

            case 1:
            graph(&scene, renderer);
            break;
        }
    }

    TTF_Quit();
    SDL_Quit();

    return 0;
}


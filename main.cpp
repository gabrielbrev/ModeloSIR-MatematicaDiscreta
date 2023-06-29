#include <iostream>
#include <cmath>
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

float window_proportion;

bool running = true;

int main(int argc, char *argv[])
{   
    SIR status = {0, 0, 0, 0};
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    SDL_Window * window = SDL_CreateWindow("MD", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    
    TTF_Font* font = TTF_OpenFont("./fonts/prototype.ttf", 50);

	if (HEIGHT < 900 || WIDTH < 1100){
		if (900 - HEIGHT <= 1100 - WIDTH){
			window_proportion = float(WIDTH) / 1100;
		}
		else{
			window_proportion = float(HEIGHT) / 900;
		}
    window_proportion = std::round(window_proportion * pow(10, 1)) / pow(10, 1);
	}
	else{
		window_proportion = 1;
	}

	int scene = 0;

    while(running)
    {   
        switch(scene){
            case 0:
            menu(&scene, renderer, font, &status);
            break;

            case 1:
            graph(&scene, renderer, font, &status);
            break;
        }
    }

    TTF_Quit();
    SDL_Quit();

    return 0;
}


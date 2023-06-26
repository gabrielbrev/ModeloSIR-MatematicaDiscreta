#include <math.h>
#include <stdio.h>
#ifdef _WIN32
  #include "src/win/SDL2/SDL.h"
  #include "src/win/SDL2/SDL_ttf.h"
#else
  #include <SDL.h>
  #include <SDL_ttf.h>
#endif

extern int WIDTH, HEIGHT;

extern bool running;

float function(float x){
  float y = pow(x, 2);
  return y;
}

void plot_function(SDL_Renderer *renderer, SDL_Point origin, int frame, float zoom){
  int y;
  for(int x = frame; x < WIDTH - frame; x++){
    y = function(x);
    SDL_RenderDrawPoint(renderer, x + origin.x, y + origin.y);
  }
}

void draw_origin(SDL_Renderer *renderer, SDL_Point origin, int frame){
  if(origin.x > frame and origin.x < WIDTH - frame){
    SDL_RenderDrawLine(renderer, origin.x, 0 + frame, origin.x, HEIGHT - frame);
  }
  if(origin.y > frame and origin.y < HEIGHT - frame){
    SDL_RenderDrawLine(renderer, 0 + frame, origin.y, WIDTH - frame, origin.y);
  }
}

int graph(int *scene, SDL_Renderer *renderer){

  SDL_Point origin = {WIDTH/2, HEIGHT/2};

  SDL_Event event;

  while(*scene == 1){
    if(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT){
        running = false;
        break;
      }
      if(event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_DOWN){
          origin.y += 5;
        }
        if(event.key.keysym.sym == SDLK_UP){
          origin.y -= 5;
        }
        if(event.key.keysym.sym == SDLK_RIGHT){
          origin.x += 5;
        }
        if(event.key.keysym.sym == SDLK_LEFT){
          origin.x -= 5;
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    draw_origin(renderer, origin, 75);
    plot_function(renderer, origin, 75, 1);

    SDL_RenderPresent(renderer);
  }

  return EXIT_SUCCESS;
}

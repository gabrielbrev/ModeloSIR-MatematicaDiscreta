#include <stdio.h>
#ifdef _WIN32
  #include "src/win/SDL2/SDL.h"
  #include "src/win/SDL2/SDL_ttf.h"
#else
  #include <SDL.h>
  #include <SDL_ttf.h>
#endif

extern bool running;

int menu(int *scene){
  SDL_Event event;
  while(*scene == 0){
    if(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT){
        running = false;
        break;
      }
    }
  }

  return EXIT_SUCCESS;
}
#include <stdio.h>
#ifdef _WIN32
  #include "../src/win/SDL2/SDL.h"
  #include "../src/win/SDL2/SDL_ttf.h"
#else
  #include <SDL.h>
  #include <SDL_ttf.h>
#endif

//a
int menu(int *scene, SDL_Renderer *renderer, TTF_Font *font, SIR *caralho);


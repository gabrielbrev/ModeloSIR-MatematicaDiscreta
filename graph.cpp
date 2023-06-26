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
	float y = pow(2, x);
	return y;
}

bool point_inframe(SDL_Point point, int frame){
	if(point.x < frame){
		return false;
	}
	else if(point.x > WIDTH - frame){
		return false;
	}
	else if(point.y < frame){
		return false;
	}
	else if(point.y > HEIGHT - frame){
		return false;
	}
	else{
		return true;
	}
}

void plot_function(SDL_Renderer *renderer, SDL_Point origin, int frame, float zoom){
	SDL_Point point;
	float y;
	for(float x = frame - origin.x; x < WIDTH - frame; x += 0.05 / zoom){
		y = function(x);
		point = {int(origin.x + x * zoom), int(origin.y + y * zoom)};
		if(point_inframe(point, frame)){
			SDL_RenderDrawPoint(renderer, int(origin.x + x * zoom), int(origin.y + y * zoom));
		}
	}
}

void draw_origin(SDL_Renderer *renderer, SDL_Point origin, int frame){
	if(origin.x > frame && origin.x < WIDTH - frame){
		SDL_RenderDrawLine(renderer, origin.x, 0 + frame, origin.x, HEIGHT - frame);
	}
	if(origin.y > frame && origin.y < HEIGHT - frame){
		SDL_RenderDrawLine(renderer, 0 + frame, origin.y, WIDTH - frame, origin.y);
	}
}

int graph(int *scene, SDL_Renderer *renderer){

	SDL_Point origin = {WIDTH/2, HEIGHT/2};

	SDL_Event event;

	float zoom = 1;

	bool reposition = true;

	while(*scene == 1){
		if(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = false;
				break;
			}
			if(event.type == SDL_KEYDOWN){
				reposition = true;
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
			if(event.type == SDL_MOUSEWHEEL){
				reposition = true;
				if(event.wheel.y > 0){
					zoom += 0.2;
				}
				if(event.wheel.y < 0){
					if(zoom - 0.2 > 0.2){
						zoom -= 0.2;
					}
				}
			}
		}

		if(reposition){
			reposition = false;

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
			draw_origin(renderer, origin, 75);

			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 100);
			plot_function(renderer, origin, 75, zoom);  
		}
		
		SDL_RenderPresent(renderer);
	}

	return 0;
}

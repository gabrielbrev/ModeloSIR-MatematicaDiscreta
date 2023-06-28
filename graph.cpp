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

float zoom = 1;

float function(float x, int option){
	float y = pow(x, 2) / 10;
	return - y;
}

bool point_in_frame(SDL_Point point, int frame){
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

void draw_dotted_function(SDL_Renderer *renderer, SDL_Point origin, int frame, float (*function)(float, int), int arg2){
	SDL_Point point;
	float y;
	for(float x = frame - origin.x; x < WIDTH - frame; x += zoom){
		y = function(x, arg2);
		point = {int(origin.x + x * zoom), int(origin.y + y * zoom)};
		if(point_in_frame(point, frame)){
			int size = 3;
			SDL_Rect rect = {point.x - size/2, point.y - size/2, size, size};
			SDL_RenderDrawRect(renderer, &rect);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

float mod(float x){
	if(x < 0){
		return -x;
	}
	else{
		return x;
	}
}

int get_decimal_places(float x){
	int decimal_places = 0;
	if(x > 1){
		while(x > 1){
			x = x * 0.1;
			decimal_places++;
		}
	}
	else{
		while(x < 1){
			x = x * 10;
			decimal_places--;
		}
	}
	return decimal_places;
}



void draw_base(SDL_Renderer *renderer, SDL_Point origin, int frame){
	int zoom_dp = get_decimal_places(zoom);
	if(zoom_dp > 2){
		zoom_dp = 2;
	}
	int marker = pow(10, mod(zoom_dp - 2));

	if(origin.x > frame && origin.x < WIDTH - frame){
		SDL_RenderDrawLine(renderer, origin.x, frame, origin.x, HEIGHT - frame);
		for(int y = frame; y < HEIGHT - frame; y++){
			if((y - origin.y) % int(marker * zoom) == 0){
				SDL_RenderDrawLine(renderer, origin.x - 10, y, origin.x + 10, y);
			}
		}
	}
	if(origin.y > frame && origin.y < HEIGHT - frame){
		SDL_RenderDrawLine(renderer, frame, origin.y, WIDTH - frame, origin.y);
		for(int x = frame; x < WIDTH - frame; x++){
			if((x - origin.x) % int(marker * zoom) == 0){
				SDL_RenderDrawLine(renderer, x, origin.y - 10, x, origin.y + 10);
			}
		}
	}
}

int graph(int *scene, SDL_Renderer *renderer){

	SDL_Point origin = {WIDTH/2, HEIGHT/2};

	SDL_Event event;

	bool reposition = true;

	SDL_StartTextInput();

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
					zoom = zoom * 1.2;
				}
				if(event.wheel.y < 0){
					zoom = 10 * (zoom / 12);
				}

				printf("zoom: %.2f\n", zoom);
				printf("thing: %d\n", get_decimal_places(zoom));
			}
			if(event.type == SDL_TEXTINPUT){
				printf("%s\n", event.text.text);
			}
		}

		if(reposition){
			reposition = false;

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
			draw_base(renderer, origin, 75);

			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 100);
			draw_dotted_function(renderer, origin, 75, function, 1);
		}
		
		SDL_RenderPresent(renderer);
	}

	return 0;
}

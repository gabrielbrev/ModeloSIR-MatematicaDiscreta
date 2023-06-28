#include <math.h>
#include <stdio.h>
#include "headers/SIR.h"
#ifdef _WIN32
	#include "src/win/SDL2/SDL.h"
	#include "src/win/SDL2/SDL_ttf.h"
#else
	#include <SDL.h>
	#include <SDL_ttf.h>
#endif

extern int WIDTH, HEIGHT;

extern bool running;

struct int_xy{
	int x;
	int y;
};
typedef int_xy ixy;

struct float_xy{
	float x;
	float y;
};
typedef float_xy fxy;

fxy zoom = {38, 0.56};

float SIR_function(float x, int option){
	SIR y;
	y.susceptible = 1199;
	y.infected = 10;
	y.contaminationRate = 0.001;
	y.recoveryRate = 0.6;
	y.days = x;
	SIR result = preGraphic(&y);
	switch(option){
		case 0:
		return - result.infected;

		case 1:
		return - result.recovered;

		case 2:
		return - result.susceptible;
	}
}

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

void draw_dotted_function(SDL_Renderer *renderer, TTF_Font *font, SDL_Point origin, int iterations, int frame, float (*function)(float, int), int arg2){
	SDL_Surface * surface = SDL_LoadBMP("./textures/dot.bmp");
	SDL_Texture * dot = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	switch(arg2){
		case 0:
		SDL_SetTextureColorMod(dot, 255, 50, 50);
		break;

		case 1:
		SDL_SetTextureColorMod(dot, 50, 255, 50);
		break;

		case 2:
		SDL_SetTextureColorMod(dot, 50, 50, 255);
	}

	SDL_Point point;
	float y;
	for(float x = 0; x < iterations; x += 1){
		y = SIR_function(x, arg2);
		point = {int(origin.x + (x * zoom.x)), int(origin.y + (y * zoom.y))};
		if(point_in_frame(point, frame)){
			int size = 13;
			SDL_Rect rect = {point.x - size/2, point.y - size/2, size, size};
			SDL_RenderCopy(renderer, dot, NULL, &rect);
		}
	}
}

void draw_base(SDL_Renderer *renderer, SDL_Point origin, int frame){
	ixy marker = {1, 100};
	int markerSize = 21;
	int thickness = 3;
	if(origin.x > frame && origin.x < WIDTH - frame){
		SDL_Rect base = {origin.x - thickness/2, frame, thickness, HEIGHT - (2 * frame)};
		SDL_RenderFillRect(renderer, &base);
		for(int y = 1; origin.y + (y * zoom.y) > frame; y--){
			if(y % marker.y == 0){
				SDL_Rect marker = {origin.x - markerSize/2, origin.y + int(y * zoom.y) - thickness/2, markerSize, thickness};
				SDL_RenderFillRect(renderer, &marker);
				SDL_Rect info = marker;
				info.w = 30;
				info.x -= info.w + 5;
				info.h = 30;
				info.y = origin.y + int(y * zoom.y) - info.h/2;
				if(info.y + info.h < origin.y)
					SDL_RenderDrawRect(renderer, &info);
			}
		}
	}
	if(origin.y > frame && origin.y < HEIGHT - frame){
		SDL_Rect base = {frame, origin.y - thickness/2, WIDTH - (2 * frame), thickness};
		SDL_RenderFillRect(renderer, &base);
		for(int x = 1; origin.x + (x * zoom.x) < WIDTH - frame; x++){
			if(x % marker.x == 0){
				SDL_Rect marker = {origin.x + int(x * zoom.x) - thickness/2, origin.y - markerSize/2, thickness, markerSize};
				SDL_RenderFillRect(renderer, &marker);
				SDL_Rect info = marker;
				info.y += info.h + 5;
				info.w = 30;
				info.h = 30;
				info.x = origin.x + int(x * zoom.x) - info.w/2;
				if(info.x > origin.x)
					SDL_RenderDrawRect(renderer, &info);
					std::string text = std::to_string(x);
					const chartextPtr = text.c_str();

					SDL_Color color = {255, 255, 255, 255};
					SDL_Surfacesurface = TTF_RenderText_Solid(font, textPtr, color);
					SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
			}
		}
	}
}

int graph(int *scene, SDL_Renderer *renderer){

	SDL_Point origin = {100, HEIGHT - 100};

	SDL_Event event;

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
		}

		if(reposition){
			reposition = false;

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
			draw_base(renderer, origin, 75);

			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 100);
			draw_dotted_function(renderer, origin, 25, 75, NULL, 0);

			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
			draw_dotted_function(renderer, origin, 25, 75, NULL, 1);

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
			draw_dotted_function(renderer, origin, 25, 75, NULL, 2);
		}
		
		SDL_RenderPresent(renderer);
	}

	return 0;
}

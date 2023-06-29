#include <math.h>
#include <stdio.h>
#include <string>
#include "headers/SIR.h"
#ifdef _WIN32
	#include "src/win/SDL2/SDL.h"
	#include "src/win/SDL2/SDL_ttf.h"
#else
	#include <SDL.h>
	#include <SDL_ttf.h>
#endif

extern int WIDTH, HEIGHT;

extern float window_proportion;

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

fxy zoom;

SIR local_status;

float SIR_function(float x, int option){
	SIR status = local_status;
	status.days = x;
	SIR result = simulate_epidemic(&status);
	switch(option){
		case 0:
		return - result.infected;

		case 1:
		return - result.recovered;
	}
	return - result.susceptible;
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

struct graph_dot_info{
	int x;
	int y;
	int type;
	SDL_Rect rect;
};
typedef graph_dot_info gdi;

void draw_dotted_function(SDL_Renderer *renderer, SDL_Point origin, gdi vec[], int iterations, int frame, float (*function)(float, int), int arg2){
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
		int size = 13 * window_proportion;
		SDL_Rect rect = {point.x - size/2, point.y - size/2, size, size};
		if(point_in_frame(point, frame))
			SDL_RenderCopy(renderer, dot, NULL, &rect);
		vec[(int)x] = {(int)x, (int)y, arg2, rect};
	}
	SDL_DestroyTexture(dot);
}

int get_decimal_places(float x){
	int count = 0;
	while(x > 1){
		x = x * 0.1;
		count++;
	}
	return count;
}

ixy set_marker(SDL_Point origin){
	ixy marker;
	uint8_t x = get_decimal_places(local_status.days) - 2;
	if(((local_status.days * zoom.x) - ((local_status.days - pow(10, x)) * zoom.x)) < (30 * window_proportion)){
		x++;
	}
	uint8_t y = get_decimal_places(local_status.susceptible + local_status.infected) - 2;
	if(((local_status.susceptible + local_status.infected) * zoom.y) - ((local_status.susceptible + local_status.infected - pow(10, y)) * zoom.y) < 30 * window_proportion){
		y++;
	}
	marker = {(int)pow(10, x), (int)pow(10, y)};
	if(x == 255){
		marker.x = 3;
	}
	if(y == 255){
		marker.y = 3;
	}
	return marker;
}

void draw_base(SDL_Renderer *renderer, SDL_Point origin, int frame, TTF_Font *font){
	ixy marker = set_marker(origin);
	int markerSize = 21 * window_proportion;
	int thickness = 3 * window_proportion;
	if(origin.x > frame && origin.x < WIDTH - frame){
		SDL_Rect base = {origin.x - thickness/2, frame, thickness, HEIGHT - (2 * frame)};
		SDL_RenderFillRect(renderer, &base);
		for(int y = 1; origin.y + (y * zoom.y) > frame; y--){
			if(y % marker.y == 0){
				SDL_Rect marker = {origin.x - markerSize/2, origin.y + int(y * zoom.y) - thickness/2, markerSize, thickness};
				SDL_RenderFillRect(renderer, &marker);
				SDL_Rect info = marker;
				info.w = 30 * window_proportion;
				info.x -= info.w + 5 * window_proportion;
				info.h = 30 * window_proportion;
				info.y = origin.y + int(y * zoom.y) - info.h/2;
				if(info.y + info.h < origin.y){
					std::string text = std::to_string(-y);
					const char *textPtr = text.c_str();

					SDL_Color color = {255, 255, 255, 255};
					SDL_Surface *surface = TTF_RenderText_Solid(font, textPtr, color);
					SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

					SDL_Rect txtr_rect = info;
					float proportion;
					if(surface->w > surface->h){
						proportion = (float)surface->h/(float)surface->w;
						txtr_rect.h = (float)txtr_rect.w * proportion;
						txtr_rect.y = info.y + info.h/2 - txtr_rect.h/2;
					}
					else{
						proportion = (float)surface->w/(float)surface->h;
						txtr_rect.w = (float)txtr_rect.h * proportion;
						txtr_rect.x = info.x + info.w/2 - txtr_rect.w/2;
					}
					SDL_FreeSurface(surface);
					SDL_RenderCopy(renderer, texture, NULL, &txtr_rect);
					SDL_DestroyTexture(texture);
				}
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
				info.y += info.h + 5 * window_proportion;
				info.w = 30 * window_proportion;
				info.h = 30 * window_proportion;
				info.x = origin.x + int(x * zoom.x) - info.w/2;
				if(info.x > origin.x){
					std::string text = std::to_string(x);
					const char *textPtr = text.c_str();

					SDL_Color color = {255, 255, 255, 255};
					SDL_Surface *surface = TTF_RenderText_Solid(font, textPtr, color);
					SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

					SDL_Rect txtr_rect = info;
					float proportion;
					if(surface->w > surface->h){
						proportion = (float)surface->h/(float)surface->w;
						txtr_rect.h = (float)txtr_rect.w * proportion;
						txtr_rect.y = info.y + info.h/2 - txtr_rect.h/2;
					}
					else{
						proportion = (float)surface->w/(float)surface->h;
						txtr_rect.w = (float)txtr_rect.h * proportion;
						txtr_rect.x = info.x + info.w/2 - txtr_rect.w/2;
					}
					SDL_FreeSurface(surface);
					SDL_RenderCopy(renderer, texture, NULL, &txtr_rect);
					SDL_DestroyTexture(texture);
				}
			}
		}
	}
}

void show_point_info(SDL_Renderer *renderer, TTF_Font *font, gdi dot, SDL_Point origin, int frame){
	struct info{
		SDL_Rect rect;
		SDL_Rect txtr_rect;
		std::string text;
	};

	info type;
	type.rect.w = 200 * window_proportion;
	type.rect.h = 50 * window_proportion;
	type.rect.x = WIDTH - type.rect.w - frame;
	type.rect.y = frame;
	SDL_Color color;
	switch(dot.type){
		case 0:
		type.text = "Infectados: ";
		color = {225, 50, 50};
		break;

		case 1:
		type.text = "Recuperados: ";
		color = {50, 225, 50};
		break;

		case 2:
		type.text = "Sucetiveis: ";
		color = {50, 50, 225};
	}
	std::string y = std::to_string(-dot.y);
	type.text += y;
	const char *textPtr = type.text.c_str();

	SDL_Surface *surface = TTF_RenderText_Solid(font, textPtr, color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

	type.txtr_rect = type.rect;
	float proportion;
	if(surface->w > surface->h){
		proportion = (float)surface->h/(float)surface->w;
		type.txtr_rect.h = (float)type.txtr_rect.w * proportion;
		type.txtr_rect.y = type.rect.y + type.rect.h/2 - type.txtr_rect.h/2;
	}
	else{
		proportion = (float)surface->w/(float)surface->h;
		type.txtr_rect.w = (float)type.txtr_rect.h * proportion;
		type.txtr_rect.x = type.rect.x + type.rect.w/2 - type.txtr_rect.w/2;
	}
	SDL_RenderCopy(renderer, texture, NULL, &type.txtr_rect);
	SDL_FreeSurface(surface);

	info day;

	day.rect.w = 200 * window_proportion;
	day.rect.h = 35 * window_proportion;
	day.rect.x = type.rect.x;
	day.rect.y = type.rect.y + type.rect.h;

	day.text = "Dia ";
	std::string x = std::to_string(dot.x);
	day.text += x;
	textPtr = day.text.c_str();

	surface = TTF_RenderText_Solid(font, textPtr, color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	day.txtr_rect = day.rect;

	proportion = (float)surface->w/(float)surface->h;
	day.txtr_rect.w = (float)day.txtr_rect.h * proportion;
	day.txtr_rect.x = day.rect.x + day.rect.w/2 - day.txtr_rect.w/2;
	
	SDL_FreeSurface(surface);

	SDL_RenderCopy(renderer, texture, NULL, &day.txtr_rect);
	SDL_RenderDrawLine(renderer, dot.rect.x, dot.rect.y + dot.rect.h/2, origin.x, dot.rect.y+ dot.rect.h/2);
}

int graph(int *scene, SDL_Renderer *renderer, TTF_Font *font, SIR *status){
	local_status = *status;
	if(local_status.days == 0){
		local_status.days = 1;
	}
	if(local_status.susceptible == 0){
		local_status.susceptible = 1;
	}

	zoom = {(float)38.0 * float(25.0)/local_status.days * window_proportion, float(0.56) * float(1200)/(local_status.susceptible + local_status.infected) * window_proportion};
	if(zoom.x == 0){
		zoom.x = 1;
	}
	if(zoom.y == 0){
		zoom.y = 1;
	}
	printf("%f : %f\n", zoom.x, zoom.y);

	SDL_Point origin = {int(100 * window_proportion), int(HEIGHT - 100 * window_proportion)};

	SDL_Event event;

	gdi susceptible[local_status.days];
	gdi infected[local_status.days];
	gdi recovered[local_status.days];

	SDL_Rect intersection;

	SDL_Rect mouse = {0, 0, 1, 1};

	int frame = int(75 * window_proportion);

	while(*scene == 1){
		if(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = false;
				break;
			}
			if(event.type == SDL_KEYDOWN){
				if(event.key.keysym.sym == SDLK_ESCAPE){
					*scene = 0;
				}
			}
			if(event.type == SDL_MOUSEMOTION){
				SDL_GetMouseState(&mouse.x, &mouse.y);
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		draw_base(renderer, origin, frame, font);

		draw_dotted_function(renderer, origin, infected, local_status.days, frame, NULL, 0);
		draw_dotted_function(renderer, origin, recovered, local_status.days, frame, NULL, 1);
		draw_dotted_function(renderer, origin, susceptible, local_status.days, frame, NULL, 2);

		for(int i = 0; i < local_status.days; i++){
			if(SDL_IntersectRect(&mouse, &susceptible[i].rect, &intersection)){
				show_point_info(renderer, font, susceptible[i], origin, frame);
				break;
			}
			else if(SDL_IntersectRect(&mouse, &recovered[i].rect, &intersection)){
				show_point_info(renderer, font, recovered[i], origin, frame);
				break;
			}
			else if(SDL_IntersectRect(&mouse, &infected[i].rect, &intersection)){
				show_point_info(renderer, font, infected[i], origin, frame);
				break;
			}
		}
		
		SDL_RenderPresent(renderer);
	}

	return 0;
}

#include <stdio.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <iostream>
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

struct textures{
    SDL_Texture *susceptible;
    SDL_Texture *contaminationRate;
    SDL_Texture *recoveryRate;
    SDL_Texture *infected;
    SDL_Texture *days;
    SDL_Texture *title;
};

bool is_number(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), [](unsigned char c) {
        return std::isdigit(c) || c == '.';
    });
}

void render_number_to_text(SDL_Renderer *renderer, float number, SDL_Rect rect, TTF_Font *font){
    std::string text = std::to_string(number);
    const char *textPtr = text.c_str();

    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, textPtr, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect txtr_rect = rect;
    float proportion;
    if(surface->w > surface->h){
        proportion = (float)surface->h/(float)surface->w;
        txtr_rect.h = (float)txtr_rect.w * proportion;
        txtr_rect.y = rect.y + rect.h/2 - txtr_rect.h/2;
    }
    else{
        proportion = (float)surface->w/(float)surface->h;
        txtr_rect.w = (float)txtr_rect.h * proportion;
        txtr_rect.x = rect.x + rect.w/2 - txtr_rect.w/2;
    }
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &txtr_rect);
    SDL_DestroyTexture(texture);
}

void load_textures(SDL_Renderer *renderer, textures *txtr, TTF_Font *font){
    SDL_Surface *surface = TTF_RenderText_Solid(font, " Numero de suscetiveis: ", {255,255,255});
    txtr->susceptible = SDL_CreateTextureFromSurface(renderer, surface);

    surface = TTF_RenderText_Solid(font, "Taxa de contaminacao:", {255,255,255});
    txtr->contaminationRate = SDL_CreateTextureFromSurface(renderer, surface);

    surface = TTF_RenderText_Solid(font, "Taxa de recuparacao:", {255,255,255});
    txtr->recoveryRate = SDL_CreateTextureFromSurface(renderer, surface);

    surface = TTF_RenderText_Solid(font, "Numero de infectados:", {255,255,255});
    txtr->infected = SDL_CreateTextureFromSurface(renderer, surface);

    surface = TTF_RenderText_Solid(font, "Numero de dias:", {255,255,255});
    txtr->days = SDL_CreateTextureFromSurface(renderer, surface);

    surface = TTF_RenderText_Solid(font, "TRABALHO FINAL", {255,0,0});
    txtr->title = SDL_CreateTextureFromSurface(renderer, surface);
}

int menu(int *scene, SDL_Renderer *renderer, TTF_Font *font, SIR *status){
    SDL_Surface *surface;

    //hitbox mouse
    SDL_Rect mouse;
    mouse.w = 1;
    mouse.h = 1;

    SDL_Rect intersection;

    //retangulo do titulo
    SDL_Rect Title;
    Title.w = 600 * window_proportion;
    Title.h = 100 * window_proportion;
    Title.x = WIDTH/2 - Title.w/2;
    Title.y = HEIGHT/32;

    SDL_Rect nameRect[5];

    //Retangulo dos Suscetiveis
    nameRect[0].w = 500 * window_proportion;
    nameRect[0].h = 80 * window_proportion;
    nameRect[0].x = WIDTH/2 - (nameRect[0].h/2) - nameRect[0].w + Title.h;
    nameRect[0].y = HEIGHT/4 - (nameRect[0].h/2);

    //retangulo da taxa de contaminacao
    nameRect[1] = nameRect[0];
    nameRect[1].y = nameRect[0].y + Title.w/10 + Title.h/20 + nameRect[0].h;

    //retangulo da taxa de recuperacao
    nameRect[2] = nameRect[0];
    nameRect[2].y = nameRect[1].y + Title.w/10 + Title.h/20 + nameRect[0].h;

    //retangulo dos infectados
    nameRect[3] = nameRect[0];
    nameRect[3].y = nameRect[2].y + Title.w/10 + Title.h/20 + nameRect[0].h;

    //retangulo dos dias
    nameRect[4] = nameRect[0];
    nameRect[4].y = nameRect[3].y + Title.w/10 + Title.h/20 + nameRect[0].h;

    SDL_Rect inputRect[5];
    //input Rect 0
    inputRect[0].w = 300 * window_proportion;
    inputRect[0].h = 80 * window_proportion;
    inputRect[0].x = WIDTH/2 + (inputRect[0].h/2) + Title.h;
    inputRect[0].y = HEIGHT/4 - (inputRect[0].h/2);

    //input Rect 1
    inputRect[1] = inputRect[0];
    inputRect[1].y = inputRect[0].y + Title.w/10 + Title.h/20 + inputRect[0].h;

    //inputRect 2
    inputRect[2] = inputRect[1];
    inputRect[2].y = inputRect[1].y + Title.w/10 + Title.h/20 + inputRect[0].h;

    //inputRect 3
    inputRect[3] = inputRect[2];
    inputRect[3].y = inputRect[2].y + Title.w/10 + Title.h/20 + inputRect[0].h;
    
    //inputRect 4
    inputRect[4] = inputRect[3];
    inputRect[4].y = inputRect[3].y + Title.w/10 + Title.h/20 + inputRect[0].h;

    textures txtr;
    load_textures(renderer, &txtr, font);

    struct input{
        int index;
        std::string text;
    };
    input input;
    SDL_StopTextInput();

    SDL_Event event;
    while(*scene == 0){
        bool leftdown = false;
        if(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
            running = false;
            break;
            }
            if(event.type == SDL_MOUSEMOTION){
                SDL_GetMouseState(&mouse.x, &mouse.y);
            }
            if(event.type == SDL_TEXTINPUT){
                if(is_number(event.text.text)){
                    input.text += event.text.text;
                    switch(input.index){
                        case 0:
                        (*status).susceptible = std::stof(input.text);
                        break;

                        case 1:
                        (*status).contaminationRate = std::stof(input.text);
                        break;

                        case 2:
                        (*status).recoveryRate = std::stof(input.text);
                        break;

                        case 3:
                        (*status).infected = std::stof(input.text);
                        break;

                        case 4:
                        (*status).days = std::stof(input.text);
                        break;
                    }
                }
            }
            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_RETURN){
                    SDL_StopTextInput();
                }
                if(event.key.keysym.sym == SDLK_BACKSPACE){
                    if(input.text.length() > 0){
                        input.text.pop_back();
                        switch(input.index){
                            case 0:
                            (*status).susceptible = std::stof(input.text);
                            break;

                            case 1:
                            (*status).contaminationRate = std::stof(input.text);
                            break;

                            case 2:
                            (*status).recoveryRate = std::stof(input.text);
                            break;

                            case 3:
                            (*status).infected = std::stof(input.text);
                            break;

                            case 4:
                            (*status).days = std::stof(input.text);
                            break;
                        }
                    }
                }
                if(event.key.keysym.sym == SDLK_p){
                    SDL_StopTextInput();
                    *scene = 1;
                }
            }
            if(event.type == SDL_MOUSEBUTTONDOWN){
                if(event.button.button == SDL_BUTTON_LEFT){
                    leftdown = true;
                }
            }
        }

        if(leftdown){
            for(int i = 0; i < 5; i++){
                if(SDL_IntersectRect(&mouse, &inputRect[i], &intersection)){
                    input.text = "0";
                    input.index = i;
                    switch(input.index){
                            case 0:
                            (*status).susceptible = std::stof(input.text);
                            break;

                            case 1:
                            (*status).contaminationRate = std::stof(input.text);
                            break;

                            case 2:
                            (*status).recoveryRate = std::stof(input.text);
                            break;

                            case 3:
                            (*status).infected = std::stof(input.text);
                            break;

                            case 4:
                            (*status).days = std::stof(input.text);
                            break;
                    }
                    SDL_StartTextInput();
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0 , 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255 , 255, 255, 255);

        //TEXTOS DOS QUADRADOS DA ESQUERDA
        SDL_RenderCopy(renderer, txtr.susceptible, NULL, &nameRect[0]);
        SDL_RenderCopy(renderer, txtr.contaminationRate, NULL, &nameRect[1]);
        SDL_RenderCopy(renderer, txtr.recoveryRate, NULL, &nameRect[2]);
        SDL_RenderCopy(renderer, txtr.infected, NULL, &nameRect[3]);
        SDL_RenderCopy(renderer, txtr.days, NULL, &nameRect[4]);
        SDL_RenderCopy(renderer, txtr.title, NULL, &Title);

        //TEXTOS DOS QUADRADOS DA DIREITA
        render_number_to_text(renderer, status->susceptible, inputRect[0], font);
        render_number_to_text(renderer, status->contaminationRate, inputRect[1], font);
        render_number_to_text(renderer, status->recoveryRate, inputRect[2], font);
        render_number_to_text(renderer, status->infected, inputRect[3], font);
        render_number_to_text(renderer, status->days, inputRect[4], font);

        for(int i = 0 ; i < 5 ; i++){
            SDL_RenderDrawRect(renderer, &inputRect[i]);
        }

        
        

        SDL_RenderPresent(renderer);

    }


    return EXIT_SUCCESS;
    }
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

bool is_number(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), [](unsigned char c) {
        return std::isdigit(c) || c == '.';
    });
}

void render_number_to_text(SDL_Renderer *renderer, float number, std::string reference, SDL_Rect rect, TTF_Font *font){
    std::string text = std::to_string(number);
    while(text.length() > reference.length()){
        text.pop_back();
        if(text.length() == 0){
            text = "0";
            break;
        }
    }
    if(text.back() == '.' && reference.back() != '.'){
        text.pop_back();
    }
    const char *textPtr = text.c_str();

    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, textPtr, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect txtr_rect = rect;
    float proportion;

    if(surface->w > surface->h && text.length() > 6){
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

void render_text(SDL_Renderer *renderer, std::string text, SDL_Rect rect, TTF_Font *font, SDL_Color color = {255, 255, 255, 255}){
    const char *textPtr = text.c_str();

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

int menu(int *scene, SDL_Renderer *renderer, TTF_Font *font, SIR *status){
    *status = {0, 0, 0, 0, 0, 0};

    SDL_Surface *surface;

    //hitbox mouse
    SDL_Rect mouse;
    mouse.w = 1;
    mouse.h = 1;

    SDL_Rect intersection;

    //retangulo do titulo
    SDL_Rect titleRect;
    titleRect.w = 600 * window_proportion;
    titleRect.h = 100 * window_proportion;
    titleRect.x = WIDTH/2 - titleRect.w/2;
    titleRect.y = HEIGHT/32;

    SDL_Rect nameRect[5];

    //Retangulo dos Suscetiveis
    nameRect[0].w = 500 * window_proportion;
    nameRect[0].h = 80 * window_proportion;
    nameRect[0].x = WIDTH/2 - (nameRect[0].h/2) - nameRect[0].w + titleRect.h;
    nameRect[0].y = HEIGHT/4 - (nameRect[0].h/2);

    int spacing = titleRect.h/4;

    //retangulo da taxa de contaminacao
    nameRect[1] = nameRect[0];
    nameRect[1].y = nameRect[0].y + spacing + nameRect[0].h;

    //retangulo da taxa de recuperacao
    nameRect[2] = nameRect[0];
    nameRect[2].y = nameRect[1].y + spacing + nameRect[0].h;

    //retangulo dos infectados
    nameRect[3] = nameRect[0];
    nameRect[3].y = nameRect[2].y + spacing + nameRect[0].h;

    //retangulo dos dias
    nameRect[4] = nameRect[0];
    nameRect[4].y = nameRect[3].y + spacing + nameRect[0].h;

    SDL_Rect inputRect[5];
    //input Rect 0
    inputRect[0].w = 300 * window_proportion;
    inputRect[0].h = 80 * window_proportion;
    inputRect[0].x = WIDTH/2 + (inputRect[0].h/2) + titleRect.h;
    inputRect[0].y = HEIGHT/4 - (inputRect[0].h/2);

    //input Rect 1
    inputRect[1] = inputRect[0];
    inputRect[1].y = nameRect[1].y;

    //inputRect 2
    inputRect[2] = inputRect[1];
    inputRect[2].y = nameRect[2].y;

    //inputRect 3
    inputRect[3] = inputRect[2];
    inputRect[3].y = nameRect[3].y;
    
    //inputRect 4
    inputRect[4] = inputRect[3];
    inputRect[4].y = nameRect[4].y;

    //Rect de início
    SDL_Rect startRect;
    startRect.w = 300 * window_proportion;
    startRect.h = 100 * window_proportion;
    startRect.x = WIDTH/2 - startRect.w/2;
    startRect.y = (nameRect[4].y + nameRect[4].h) + startRect.h/2;

    struct input{
        int index;
        std::string text[5];
        bool hasdot[5];
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
                    input.text[input.index] += event.text.text;
                    if(std::stoi(input.text[input.index]) > 9999){
                        input.text[input.index].pop_back();
                    }
                    if(event.text.text[0] == '.'){
                        if(input.hasdot[input.index]){
                            input.text[input.index].pop_back();
                        }
                        else{
                            input.hasdot[input.index] = true;
                        }
                    }
                    switch(input.index){
                        case 0:
                        status->susceptible = std::stof(input.text[input.index]);
                        break;

                        case 1:
                        status->contaminationRate = std::stof(input.text[input.index]);
                        break;

                        case 2:
                        status->recoveryRate = std::stof(input.text[input.index]);
                        break;

                        case 3:
                        status->infected = std::stof(input.text[input.index]);
                        break;

                        case 4:
                        status->days = std::stof(input.text[input.index]);
                        break;
                    }
                }
            }
            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_RETURN){
                    SDL_StopTextInput();
                }
                if(event.key.keysym.sym == SDLK_BACKSPACE){
                    if(input.text[input.index].length() > 1){
                        if(input.text[input.index].back() == '.'){
                            input.hasdot[input.index] = false;
                        }
                        input.text[input.index].pop_back();
                        switch(input.index){
                            case 0:
                            status->susceptible = std::stof(input.text[input.index]);
                            break;

                            case 1:
                            status->contaminationRate = std::stof(input.text[input.index]);
                            break;

                            case 2:
                            status->recoveryRate = std::stof(input.text[input.index]);
                            break;

                            case 3:
                            status->infected = std::stof(input.text[input.index]);
                            break;

                            case 4:
                            status->days = std::stoi(input.text[input.index]);
                            break;
                        }
                    }
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

                    input.index = i;
                    input.text[input.index] = "0";
                    input.hasdot[input.index] = false;
                    switch(input.index){
                        case 0:
                        status->susceptible = std::stof(input.text[input.index]);
                        break;

                        case 1:
                        status->contaminationRate = std::stof(input.text[input.index]);
                        break;

                        case 2:
                        status->recoveryRate = std::stof(input.text[input.index]);
                        break;

                        case 3:
                        status->infected = std::stof(input.text[input.index]);
                        break;

                        case 4:
                        status->days = std::stoi(input.text[input.index]);
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
        render_text(renderer, "Numero de suscetiveis:", nameRect[0], font);
        render_text(renderer, " Taxa de contaminacao:", nameRect[1], font);
        render_text(renderer, "  Taxa de recuperacao:", nameRect[2], font);
        render_text(renderer, " Numero de infectados:", nameRect[3], font);
        render_text(renderer, "       Numero de dias:", nameRect[4], font);

        //TEXTOS DOS QUADRADOS DA DIREITA
        render_number_to_text(renderer, status->susceptible, input.text[0], inputRect[0], font);
        render_number_to_text(renderer, status->contaminationRate, input.text[1], inputRect[1], font);
        render_number_to_text(renderer, status->recoveryRate, input.text[2], inputRect[2], font);
        render_number_to_text(renderer, status->infected, input.text[3], inputRect[3], font);
        render_number_to_text(renderer, status->days, input.text[4], inputRect[4], font);


        render_text(renderer, "Modelo SIR", titleRect, font, {255, 40, 40, 255});
        render_text(renderer, "simular", startRect, font, {255, 100, 100, 255});

        if(SDL_IntersectRect(&mouse, &startRect, &intersection)){
            render_text(renderer, "simular", startRect, font, {100, 100, 100, 155});
            if(leftdown){
                *scene = 1;
            }
        }

        SDL_SetRenderDrawColor(renderer, 70, 70, 70, 70);
        for(int i = 0 ; i < 5 ; i++){
            SDL_RenderDrawRect(renderer, &inputRect[i]);
        }

        SDL_RenderPresent(renderer);

    }


    return EXIT_SUCCESS;
    }
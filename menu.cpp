#include <stdio.h>
#include <string>
#include <sstream>
#include <iomanip>
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



//a
int menu(int *scene, SDL_Renderer *renderer, TTF_Font *font, SIR *caralho){


    //hitbox mouse
    SDL_Rect mouse;
    mouse.w = 1;
    mouse.h = 1;

    //retangulo do titulo
    SDL_Rect Title;
    Title.w = 600;
    Title.h = 100;
    Title.x = WIDTH/2 - Title.w/2;
    Title.y = HEIGHT/32;

    SDL_Rect nameRect[5];

    //Retangulo dos Suscetiveis
    nameRect[0].w = 500;
    nameRect[0].h = 80;
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
    inputRect[0].w = 300;
    inputRect[0].h = 80;
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

    bool done = false;
    int info = 0;
    SDL_Event event;
    while(*scene == 0){
        if(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
            running = false;
            break;
            }
            if(event.type == SDL_MOUSEMOTION){
            SDL_GetMouseState(&mouse.x, &mouse.y);
            mouse.x -= mouse.w/2;
            mouse.y -= mouse.h/2;
            }
        }

        //TEXTOS DOS QUADRADOS DA ESQUERDA
        SDL_SetRenderDrawColor(renderer, 255 , 255, 255, 255);
        SDL_RenderDrawRect(renderer, &Title);
        SDL_Surface* surfaceText = TTF_RenderText_Solid(font, " Numero de suscetiveis: ", {255,255,255});
        SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
        SDL_RenderCopy(renderer, textureText, NULL, &nameRect[0]);
        surfaceText = TTF_RenderText_Solid(font, " Taxa de contaminacao: ", {255,255,255});
        textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
        SDL_RenderCopy(renderer, textureText, NULL, &nameRect[1]);
        surfaceText = TTF_RenderText_Solid(font, " Taxa de recuparacao: ", {255,255,255});
        textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
        SDL_RenderCopy(renderer, textureText, NULL, &nameRect[2]);
        surfaceText = TTF_RenderText_Solid(font, " Numero de infectados: ", {255,255,255});
        textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
        SDL_RenderCopy(renderer, textureText, NULL, &nameRect[3]);
        surfaceText = TTF_RenderText_Solid(font, " Numero de dias: ", {255,255,255});
        textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
        SDL_RenderCopy(renderer, textureText, NULL, &nameRect[4]);
        surfaceText = TTF_RenderText_Solid(font, "TRABALHO FINAL", {255,0,0});
        textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
        SDL_RenderCopy(renderer, textureText, NULL, &Title);

        

        for(int i = 0 ; i < 5 ; i++){
            SDL_RenderDrawRect(renderer, &inputRect[i]);
            SDL_RenderDrawRect(renderer, &nameRect[i]);
        }
        SDL_RenderPresent(renderer);
        // std::stringstream ssusceptible;
        // std::stringstream scontaminationRate;
        // std::stringstream srecoveryRate;
        // std::stringstream sinfected;
        // std::stringstream sdays;

        
        if(!done){
          
          switch (info)
          {
          case 0:
            printf("Quantos suscetiveis temos?\n");
            scanf("%f", &(*caralho).susceptible);
            //ssusceptible << (*caralho).susceptible;
            break;
          case 1:
            printf("Qual a taxa de contaminacao?\n");
            scanf("%f", &(*caralho).contaminationRate);
            //scontaminationRate << (*caralho).contaminationRate;
            break;
          case 2:
            printf("qual a taxa de recuperacao?\n");
            scanf("%f", &(*caralho).recoveryRate);
            //srecoveryRate << (*caralho).recoveryRate;
            break;
          case 3:
            printf("Quantos infectados no inicio?\n");
            scanf("%f", &(*caralho).infected);
            //sinfected << (*caralho).infected;
            break;
          case 4:
            printf("Quantos dias se passaram?\n");
            scanf("%d", &(*caralho).days);
            //sdays << (*caralho).days;
            //a
          default:
            break;
          }
        }
        //TENTATIVAS DE RESTRINGIR AS CASAS DECIMAIS DOS FLOATS CONVERTIDOS PARA STRING
        // std::ostringstream oss;
        // oss << std::fixed << std::setprecision(2) << (*caralho).susceptible;
        // std::string stringsusceptible = oss.str();
        // const char* textPtr = stringsusceptible.c_str();


        std::string stringsusceptible = std::to_string(caralho->susceptible);
        const char *textPtr = stringsusceptible.c_str();

        surfaceText = TTF_RenderText_Solid(font, textPtr, {255,255,255});
        textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
        SDL_RenderCopy(renderer, textureText, NULL, &inputRect[0]);

         std::string stringContaminationRate = std::to_string(caralho->contaminationRate);
         textPtr = stringContaminationRate.c_str();
        // std::ostringstream oss2;
        // oss2 << std::fixed << std::setprecision(3) << (*caralho).contaminationRate;
        // std::string stringContaminationRate = oss2.str();
        // textPtr = stringContaminationRate.c_str();

        surfaceText = TTF_RenderText_Solid(font, textPtr, {255,255,255});
        textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
        SDL_RenderCopy(renderer, textureText, NULL, &inputRect[1]);

        std::string stringRecoveryRate = std::to_string(caralho->recoveryRate);
        textPtr = stringRecoveryRate.c_str();

        // std::ostringstream oss3;
        // oss3 << std::fixed << std::setprecision(2) << (*caralho).recoveryRate;
        // std::string stringRecoveryRate = oss3.str();
        // textPtr = stringRecoveryRate.c_str();

        surfaceText = TTF_RenderText_Solid(font, textPtr, {255,255,255});
        textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
        SDL_RenderCopy(renderer, textureText, NULL, &inputRect[2]);

        std::string stringInfected = std::to_string(caralho->infected);
        textPtr = stringInfected.c_str();

        // std::ostringstream oss4;
        // oss4 << std::fixed << std::setprecision(0) << (*caralho).infected;
        // std::string stringInfected= oss4.str();
        // textPtr = stringInfected.c_str();

        surfaceText = TTF_RenderText_Solid(font, textPtr, {255,255,255});
        textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
        SDL_RenderCopy(renderer, textureText, NULL, &inputRect[3]);

        std::string stringDays = std::to_string((float)caralho->days);
        textPtr = stringDays.c_str();

        // std::ostringstream oss5;
        // oss5 << std::fixed << std::setprecision(0) << (float)(caralho->days);
        // std::string stringDays= oss5.str();
        // textPtr = stringDays.c_str();

        surfaceText = TTF_RenderText_Solid(font, textPtr, {255,255,255});
        textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
        SDL_RenderCopy(renderer, textureText, NULL, &inputRect[4]);


        info++;
        if(info == 5)
          *scene = 1;
    }


    return EXIT_SUCCESS;
    }
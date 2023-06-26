#include <math.h>
#include <stdio.h>
#include <iostream>


void preGraphic(float susceptible, float a, float b, int days);
void nInfected(float *infected , float peopleSusceptible, float a, float b);
void nSusceptible(float peopleInfected, float *susceptible, float a);
void nRecovered(float *recovered, float peopleInfected, float b);



struct SIR{
    float susceptible;
    float infected;
    float recovered;
};


int main(){
    SIR question;
    float susceptible;
    int days;
    float a;
    float b;
    int option;
    printf("Quantas sao as pessoas suscetiveis?\n");
    scanf("%f", &question.susceptible);
    printf("Quantas sao as pessoas infectadas?\n");
    scanf("%f", &question.infected);
    printf("Qual eh a taxa (a) de infeccao da doenca?\n");
    scanf("%f", &a);
    printf("Qual eh a taxa (b) de recuperacao da doenca?\n");
    scanf("%f", &b);
    printf("Qual o numero de dias que deseja saber?\n");
    scanf("%d", &days);
    preGraphic(question, a , b , days, option);
}


SIR preGraphic(SIR question, float a, float b, int days, int option){
    question.recovered = 0;
    printf("No inicio, tinham quantos infectados?\n");
    scanf("%f" , &question.infected);
    
    
    for(int n = 0 ; n <= days ; n++)
    {   
        float peopleSusceptible = question.susceptible;
        float peopleInfected = question.infected;
        nInfected(&question.infected, peopleSusceptible, a, b);
        nSusceptible(&question.susceptible, peopleInfected, a);
        nRecovered(&question.recovered, peopleInfected, b);
        //printf("Dia %d | %d Pessoas suscetiveis | %d Pessoas infectadas | %d Pessoas recuperadas\n", n, (int)susceptible, (int)infected, (int)recovered);
    }
    return question;
}


void nInfected(float *infected , float peopleSusceptible, float a, float b){
    *infected = *infected + (a*((peopleSusceptible)*(*infected))) - b*(*infected);
}


void nSusceptible(float *susceptible, float peopleInfected, float a){
    *susceptible = *susceptible - a*((*susceptible)*(peopleInfected));
}


void nRecovered(float *recovered, float peopleInfected, float b){
    *recovered = *recovered + b*(peopleInfected);
}








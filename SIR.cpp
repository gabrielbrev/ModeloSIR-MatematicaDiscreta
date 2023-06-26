#include <math.h>
#include <stdio.h>
#include <iostream>
//TRAVALHO FINAL
struct SIR{
    float susceptible;
    float infected;
    float recovered;
    float contaminationRate;
    float recoveryRate;
    int days;
};

SIR preGraphic(SIR *result, int option);
void nInfected(SIR *info , float peopleSusceptible, SIR result);
void nSusceptible(SIR *info, float peopleInfected, SIR result);
void nRecovered(SIR *info, float peopleInfected, SIR result);

//main substituivel para passar a struct por parametros ( main meramente ilustrativa )
int main(){
    SIR question;
    int option;
    printf("Quantas sao as pessoas suscetiveis?\n");
    scanf("%f", &question.susceptible);
    printf("Quantas sao as pessoas infectadas?\n");
    scanf("%f", &question.infected);
    printf("Qual eh a taxa de contaminacao da doenca?\n");
    scanf("%f", &question.contaminationRate);
    printf("Qual eh recoveryRate taxa (b) de recuperacao da doenca?\n");
    scanf("%f", &question.recoveryRate);
    printf("Qual o dia voce quer?\n");
    scanf("%d", &question.days);
    question.recovered = 0;
    preGraphic(&question, option);
    printf("%d" , (int)question.susceptible);
}

// funcao para fazer o grafico e retornar o resultado das funcoes selecionadas
SIR preGraphic(SIR *result, int option){
    
    
    if(result->days == 0)
    {
        //printf("Dia %d | %d Pessoas suscetiveis | %d Pessoas infectadas | %d Pessoas recuperadas\n", result->days, (int)result->susceptible, (int)result->infected, (int)result->recovered);
        return *result;
    }
    for(int n = 1 ; n <= result->days ; n++)
    {   
        float peopleSusceptible = result->susceptible;
        float peopleInfected = result->infected;
        nInfected(&(*result), peopleSusceptible, *result);
        nSusceptible(&(*result), peopleInfected, *result);
        nRecovered(&(*result), peopleInfected, *result);
        printf("Dia %d | %d Pessoas suscetiveis | %d Pessoas infectadas | %d Pessoas recuperadas\n", n, (int)result->susceptible, (int)result->infected, (int)result->recovered);
    }
    return *result;
}

//funcao dos infectados
void nInfected(SIR *info , float peopleSusceptible, SIR result){
    info->infected = info->infected + (result.contaminationRate*((peopleSusceptible)*(info->infected))) - result.recoveryRate*(info->infected);
}

//funcao dos suscetiveis
void nSusceptible(SIR *info, float peopleInfected, SIR result){
    info->susceptible = info->susceptible - result.contaminationRate*((info->susceptible)*(peopleInfected));
}

//funcao dos recuperados
void nRecovered(SIR *info, float peopleInfected, SIR result){
    info->recovered = info->recovered + result.recoveryRate*(peopleInfected);
}








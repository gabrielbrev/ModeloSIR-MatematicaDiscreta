#include <math.h>
#include <stdio.h>
#include <iostream>


struct SIR{
    float susceptible;
    float infected;
    float recovered;
    float contaminationRate;
    float recoveryRate;
    int days;
};


//funcao de calculo de infectados
void nInfected(SIR *info , float peopleSusceptible, SIR result){
    info->infected = info->infected + (result.contaminationRate*((peopleSusceptible)*(info->infected))) - result.recoveryRate*(info->infected);
}

//funcao de calculo dos suscetiveis
void nSusceptible(SIR *info, float peopleInfected, SIR result){
    info->susceptible = info->susceptible - result.contaminationRate*((info->susceptible)*(peopleInfected));
}

//funcao de calculo dos recuperados
void nRecovered(SIR *info, float peopleInfected, SIR result){
    info->recovered = info->recovered + result.recoveryRate*(peopleInfected);
}


//funcao numerica do grafico.
SIR preGraphic(SIR *result){
    
    
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






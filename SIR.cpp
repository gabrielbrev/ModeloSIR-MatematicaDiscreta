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
void nInfected(SIR *info , float peopleSusceptible, SIR status){
    info->infected = info->infected + (status.contaminationRate*((peopleSusceptible)*(info->infected))) - status.recoveryRate*(info->infected);
}

//funcao de calculo dos suscetiveis
void nSusceptible(SIR *info, float peopleInfected, SIR status){
    info->susceptible = info->susceptible - status.contaminationRate*((info->susceptible)*(peopleInfected));
}

//funcao de calculo dos recuperados
void nRecovered(SIR *info, float peopleInfected, SIR status){
    info->recovered = info->recovered + status.recoveryRate*(peopleInfected);
}

//funcao numerica do grafico.
SIR simulate_epidemic(SIR *status){
    if(status->days == 0)
    {
        return *status;
    }
    for(int n = 1 ; n <= status->days ; n++)
    {   
        float peopleSusceptible = status->susceptible;
        float peopleInfected = status->infected;
        nInfected(&(*status), peopleSusceptible, *status);
        nSusceptible(&(*status), peopleInfected, *status);
        nRecovered(&(*status), peopleInfected, *status);
    }
    return *status;
}







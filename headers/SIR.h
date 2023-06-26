


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
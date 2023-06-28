
struct SIR{
    float susceptible;
    float infected;
    float recovered;
    float contaminationRate;
    float recoveryRate;
    int days;
};
//a
SIR preGraphic(SIR *result);
void nInfected(SIR *info , float peopleSusceptible, SIR result);
void nSusceptible(SIR *info, float peopleInfected, SIR result);
void nRecovered(SIR *info, float peopleInfected, SIR result);
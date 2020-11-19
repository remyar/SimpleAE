#include "./engine.h"

static unsigned long prec_H = 0; //Heure du front precedent en µs
static unsigned long T = 0;      //Periode en cours
static bool Mot_OFF = 0;         //Sera 1 si moteur detecté arrété par l'isr_GestionIbob()
static unsigned long Tdem = 0;   //Periode correspondante à Ndem,forcée pour le premier tour
static int Ndem = 60;            //Vitesse estimée du vilo entrainé par le demarreur en t/mn
static float NT;

static uint8_t nbCylindres;

void ISR_handle(void)
{
    T = micros() - prec_H; //front actif, arrivé calculer T
    prec_H = micros();     //heure du front actuel qui deviendra le front precedent
    if (Mot_OFF == 1)
    {                          //Demarrage:premier front de capteur
        T = Tdem;              //Fournir  T = Tdem car prec_H n'existe par pour la première étincelle
        GPIO_BOBINE_ON();      //Alimenter la bobine
        GPIO_BUILTIN_LED_ON(); //Temoin
        Mot_OFF = 0;           //Le moteur tourne
    }
    if (T > (unsigned long)COURBE_GetLimit()) //Sous la ligne rouge?
    {
        // Top();  //Oui, generer une etincelle
        Mot_OFF = SPARK_Run(COURBE_TaskRun(T), T);
    }
}

uint8_t ENGINE_GetNbCylindres(void){
    return nbCylindres;
}

void ENGINE_SetNbCylindres(uint8_t val){
    nbCylindres = val;
}

void ENGINE_TaskInit(void)
{
    COURBE_Init();
    SPARK_Init();
    BOBINE_Init();

    nbCylindres = MEMORY_ReadByte(ADDR_NB_CYLINDRES);

    attachInterrupt(digitalPinToInterrupt(GPIO_PIN_CIBLE), ISR_handle, CAPTEUR_STATE_ON ? FALLING : RISING);

    NT = (float)120000000 / (float)nbCylindres; //Facteur de conversion Nt/mn moteur, Tµs entre deux PMH étincelle
    Tdem = NT / Ndem;                            //Periode imposée à  la première étincelle qui n'a pas de valeur prec_H

    Mot_OFF = 1;
}
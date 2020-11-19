#include "./spark.h"
#include "./engine.h"

static uint32_t Tprec;
//Si Dwell=2, temps de recharge bobine, 3ms= 3000µs typique, 7ms certaines motos
const int trech = 3000;
static unsigned long Ttrans; //T transition de Dwell 4
const int Ntrans = 3000;     //Regime de transition et maxi affichage RPM Sphone
const int tetin = 500;       //Typique 500 à 1000µs, durée etincelle regimes >Ntrans
static uint32_t T;

void SPARK_Init(void)
{
    Tprec = 0;

    float NT = (float)120000000 / (float)ENGINE_GetNbCylindres(); //Facteur de conversion Nt/mn moteur, Tµs entre deux PMH étincelle
    Ttrans = NT / Ntrans;                              //Calcul de la periode de transition pour Dwell 4
}

void ISR_Spark(void)
{
    unsigned long Davant_rech = 0; //Delai en µs avant la recharge de la  bobine.
    GPIO_BOBINE_OFF();             //Couper le courant, donc étincelle
    GPIO_BUILTIN_LED_OFF();        //Temoin
    //Maintenant que l'étincelle est émise, il faut rétablir Ibob au bon moment

    switch (DWELL_MODE) //Attente courant coupé selon le type de Dwell
    {
    case 1:                 //Ibob coupe 1ms par cycle seulement, la bobine doit supporter de chauffer
        Davant_rech = 1000; //1ms off par cycle
        break;
    case 2:                                  //Type bobine faible resistance, dite "electronique"
        Davant_rech = 2 * T - Tprec - trech; //On doit tenir compte des variations de régime moteur
        Tprec = T;                           //Maj de la future periode precedente
        break;
    case 3: //Type "vis platinées", Off 1/3, On 2/3
        Davant_rech = T / 3;
        break;
    case 4: //Type optimisé haut régime
        if (T > Ttrans)
            Davant_rech = T / 3; // En dessous de N trans, typique 3000t/mn
        else
            Davant_rech = tetin; // Au delà de Ntrans, on limite la durée de l'étincelle, typique 0.5ms
        break;
    }

    BOBINE_SetTime(Davant_rech);
    BOBINE_SetState(true);
}

bool SPARK_Run(uint32_t D, uint32_t _T)
{
    T = _T;
    Timer1.attachInterrupt(ISR_Spark);
    Timer1.initialize(D);
    return true;
}
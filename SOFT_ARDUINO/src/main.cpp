
//*****************************************************************************

#include "./Low-Level/board.h"
#include "./compc/COM_PC.h"
#include "./courbe.h"
#include "./bobine.h"
#include "./spark.h"


//***********************Variables du sketch************************************
static unsigned long prec_H = 0;      //Heure du front precedent en µs
static unsigned long T = 0;           //Periode en cours
static bool Mot_OFF = 0;               //Sera 1 si moteur detecté arrété par l'isr_GestionIbob()
static unsigned long Tdem = 0;    //Periode correspondante à Ndem,forcée pour le premier tour
static int Ndem = 60;                 //Vitesse estimée du vilo entrainé par le demarreur en t/mn
static float NT;

//********************LES FONCTIONS*************************


////////////////////////////////////////////////////////////////////////
void setup() ///////////////
/////////////////////////////////////////////////////////////////////////
{
    BOARD_Init();  

    COURBE_Init();  
    SPARK_Init();
    BOBINE_Init();
    COMPC_TaskInit();

    NT = (float)120000000 / (float)NB_CYLINDRES;    //Facteur de conversion Nt/mn moteur, Tµs entre deux PMH étincelle
    Tdem = NT / Ndem;                         //Periode imposée à  la première étincelle qui n'a pas de valeur prec_H

    Mot_OFF = 1;
}

///////////////////////////////////////////////////////////////////////////
void loop() ////////////////
////////////////////////////////////////////////////////////////////////////
{
    while (GPIO_READ(GPIO_PIN_CIBLE) == !CAPTEUR_STATE_ON)
    {
        //Attendre front actif de la cible
        COMPC_TaskRun();
    }

    T = micros() - prec_H; //front actif, arrivé calculer T
    prec_H = micros();     //heure du front actuel qui deviendra le front precedent
    if (Mot_OFF == 1)
    {                          //Demarrage:premier front de capteur
        T = Tdem;              //Fournir  T = Tdem car prec_H n'existe par pour la première étincelle
        GPIO_BOBINE_ON();  //Alimenter la bobine
        GPIO_BUILTIN_LED_ON(); //Temoin
        Mot_OFF = 0;           //Le moteur tourne
    }
    if (T > (unsigned long)COURBE_GetLimit()) //Sous la ligne rouge?
    {
        // Top();  //Oui, generer une etincelle
        Mot_OFF = SPARK_Run(COURBE_TaskRun(T) , T);
    }

    if ( T > (NT / 3000) ){
        Serial.println(T, 1);
    }

    while (GPIO_READ(GPIO_PIN_CIBLE) == CAPTEUR_STATE_ON)
    {
        //Attendre si la cible encore active
        COMPC_TaskRun();
    }
}


/////////////////Exemples de CAPTEURS/////////////////
//Capteur Honeywell cylindrique 1GT101DC,contient un aimant sur le coté,type non saturé, sortie haute à vide,
//et basse avec une cible en acier. Il faut  CapteurOn = 0, declenchement sur front descendant.
//Le capteur à fourche SR 17-J6 contient un aimant en face,type saturé, sortie basse à vide,
//et haute avec une cible en acier. Il faut  CapteurOn = 1, declenchement sur front montant.

//Pour les Ncyl pairs:2,4,6,8,10,12, le nombre de cibles réparties sur le vilo est Ncyl/2
//Dans les deux cas (capteur sur vilo ou dans l'alumeur) la periode entre deux cibles et la même car l'AàC tourne à Nvilo/2
//Pour les Ncyl impairs 1,3 5, 7?,9? il FAUT un capteur dans l'alumeur (ou AàC)


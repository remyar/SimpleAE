#include "./bobine.h"
#include "./courbe.h"

static bool sparkState = true;

void ISR_Bobine(void)
{
    Timer1.stop(); //Arreter le decompte du timer
    if (sparkState == true)
    {
        GPIO_BOBINE_ON();  //Le moteur tourne,retablire le courant dans bobine
        GPIO_BUILTIN_LED_ON(); //Temoin
    }
    else
    {
        GPIO_BOBINE_OFF();
        GPIO_BUILTIN_LED_OFF(); //Temoin//Moteur arrete, preserver la bobine, couper le courant
        COURBE_TaskRun(0);
    }
    sparkState = false;              //Remet  le detecteur d'étincelle à 0
    Timer1.initialize(SECU_BOBINE);
}

void BOBINE_Init(void)
{
    Timer1.attachInterrupt(ISR_Bobine); //IT d'overflow de Timer1 (16 bits)
    Timer1.initialize(SECU_BOBINE);     //Le courant dans la bobine sera coupé si aucune etincelle durant Dsecu µs
}

void BOBINE_SetState(bool state){
    sparkState = state;
}

void BOBINE_SetTime( unsigned long  Davant_rech){
    Timer1.attachInterrupt(ISR_Bobine);
    Timer1.initialize(Davant_rech);
}
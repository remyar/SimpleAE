//================================================================================================//
//                                                                                                //
// PROJET       : DongleWifi GoodRace                                                             //
// MODULE       : Board                                                                           //
// DESCRIPTION  :                                                                                 //
// CREATION     : 27/01/2020                                                                      //
// HISTORIQUE   :                                                                                 //
//                                                                                                //
//================================================================================================//

//================================================================================================//
//                                        FICHIERS INCLUS                                         //
//================================================================================================//

#include "board.h"
#include "Serial.h"

//================================================================================================//
//                                            DEFINES                                             //
//================================================================================================//

//================================================================================================//
//                                          ENUMERATIONS                                          //
//================================================================================================//

//================================================================================================//
//                                      STRUCTURES ET UNIONS                                      //
//================================================================================================//

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 VARIABLES PRIVEES ET PARTAGEES                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------//
//---                                         Privees                                          ---//
//------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FONCTIONS PRIVEES ET PARTAGEES                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------//
//---                                         Privees                                          ---//
//------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------
// FONCTION    : _InitClocks
//
// DESCRIPTION : Initialisation des clocks du micro
//--------------------------------------------------------------------------------------------------
static void _InitClocks(void)
{
}

void delayMilliseconds(uint16_t _ms)
{
    for (uint16_t i = 0; i < _ms; i++)
    {
        delayMicroseconds(1000);
    }
}
//--------------------------------------------------------------------------------------------------
// FONCTION    : _InitGpio
//
// DESCRIPTION : Initiliasation des GPIO
//--------------------------------------------------------------------------------------------------
static void _InitGpio(void)
{
    pinMode(GPIO_PIN_BUILTIN_LED, OUTPUT); //Led d'origine sur tout Arduino, temoin du courant dans la bobine
    pinMode(GPIO_PIN_CIBLE, INPUT_PULLUP); //Entrée front du capteur sur D2
    pinMode(GPIO_PIN_BOBINE, OUTPUT);      //Sortie sur D4 controle du courant dans la bobine
    pinMode(GPIO_PIN_POT, INPUT_PULLUP);   //Entrée pour potard 100kohms, optionnel

    GPIO_BUILTIN_LED_OFF();
    GPIO_BOBINE_OFF(); //par principe, couper la bobine
}

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------
// FONCTION    : BOARD_Init
//
// DESCRIPTION : Initialisation de la carte : GPIO, Clocks, Interruptions...
//--------------------------------------------------------------------------------------------------
void BOARD_Init(void)
{
    //--- Initialisation des clocks
    _InitClocks();

    //--- Initialisation des GPIO
    _InitGpio();

    SERIAL_Init();

    MEMORY_Init();
}

//================================================================================================//
//                                                                                                //
// PROJET       : DBM620                                                                          //
// MODULE       : Board                                                                           //
// DESCRIPTION  :                                                                                 //
// CREATION     : 18/12/2017                                                                      //
// HISTORIQUE   :                                                                                 //
//                                                                                                //
//================================================================================================//

#ifndef _BOARD_
#define _BOARD_

//================================================================================================//
//                                        FICHIERS INCLUS                                         //
//================================================================================================//

#include <Arduino.h>
#include "./memory.h"
//================================================================================================//
//                                            DEFINES                                             //
//================================================================================================//
#define POSITION_CAPTEUR 45 //Position en degrès vilo du capteur( Hall ou autre ) AVANT le PMH étincelle du cylindre N°1
#define SECU_BOBINE 1000000 //Securite: bobine coupee à l'arret apres Dsecu µs
#define DWELL_MODE  1           //Dwell = 2 pour alimentation de la bobine seulement trech ms par cycle, 3ms par exemple
                                //Obligatoire pour bobine 'electronique'   de faible resistance: entre 2 et 0.5ohm.Ajuster  trech
                                //Dwell = 3 pour simuler un allumage à vis platinées: bobine alimentée 2/3 (66%) du cycle
                                //Dwell = 4 pour optimiser l'étincelle à haut régime.La bobine chauffe un peu plus.

#define CAPTEUR_STATE_ON    0

#define GPIO_PIN_BUILTIN_LED 13
#define GPIO_PIN_CIBLE 2
#define GPIO_PIN_BOBINE 4 //Sortie D4 vers bobine.En option, on peut connecter une Led avec R=330ohms vers la masse
#define GPIO_PIN_POT A0   //Entrée analogique sur A0 pour potard de changement de courbes. R PullUp

#define GPIO_BOBINE_ON() digitalWrite(GPIO_PIN_BOBINE, HIGH)
#define GPIO_BOBINE_OFF() digitalWrite(GPIO_PIN_BOBINE, LOW)
#define GPIO_BOBINE(X) digitalWrite(GPIO_PIN_BOBINE, X)

#define GPIO_BUILTIN_LED_ON() digitalWrite(GPIO_PIN_BUILTIN_LED, HIGH)
#define GPIO_BUILTIN_LED_OFF() digitalWrite(GPIO_PIN_BUILTIN_LED, LOW)
#define GPIO_BUILTIN_LED(X) digitalWrite(GPIO_PIN_BUILTIN_LED, X)

#define GPIO_READ(X) digitalRead(X)
#define GPIO_WRITE(X, Y) digitalWrite(X, Y)
#define ANALOG_READ(X) analogRead(X)

//================================================================================================//
//                                          ENUMERATIONS                                          //
//================================================================================================//

//================================================================================================//
//                                      STRUCTURES ET UNIONS                                      //
//================================================================================================//

//================================================================================================//
//                                VARIABLES ET FONCTION PARTAGEES                                 //
//================================================================================================//

//------------------------------------------------------------------------------------------------//
//---                                        Variables                                         ---//
//------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------//
//---                                        Fonctions                                         ---//
//------------------------------------------------------------------------------------------------//
void delayMilliseconds(uint16_t _ms);
void BOARD_Init(void);

#endif //--- _BOARD_

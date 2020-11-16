//================================================================================================//
//                                                                                                //
// PROJET       : DBM620                                                                          //
// MODULE       : WiFi                                                                           //
// DESCRIPTION  :                                                                                 //
// CREATION     : 18/12/2017                                                                      //
// HISTORIQUE   :                                                                                 //
//                                                                                                //
//================================================================================================//

#ifndef _SERIAL_H_
#define _SERIAL_H_

//================================================================================================//
//                                        FICHIERS INCLUS                                         //
//================================================================================================//

#include "board.h"

//================================================================================================//
//                                            DEFINES                                             //
//================================================================================================//

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
void SERIAL_Init(void);
bool SERIAL_Available(void);
unsigned char SERIAL_Getc(void);
void SERIAL_PutString(String str);
void SERIAL_Putc(unsigned char data);
#endif //--- _BOARD_

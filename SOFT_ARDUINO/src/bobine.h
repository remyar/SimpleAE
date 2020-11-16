#ifndef _BOBINE_H_
#define _BOBINE_H_

#include <TimerOne.h>
#include "./Low-Level/board.h"

void BOBINE_Init(void);
void BOBINE_SetState(bool state);
void BOBINE_SetTime( unsigned long  Davant_rech);

#endif

#ifndef _COURBE_H_
#define _COURBE_H_

#include "./Low-Level/board.h"

void COURBE_Init(void);
void COURBE_Select(uint8_t idx);
float COURBE_GetLimit(void);
uint32_t COURBE_TaskRun(uint32_t);
float COURBE_GetRpm(void);
float COURBE_GetDelay(void);

#endif
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "./Low-Level/board.h"
#include "./Low-Level/memory.h"
#include "./courbe.h"
#include "./bobine.h"
#include "./spark.h"

uint8_t ENGINE_GetNbCylindres(void);
void ENGINE_SetNbCylindres(uint8_t val);

void ENGINE_TaskInit(void);


#endif
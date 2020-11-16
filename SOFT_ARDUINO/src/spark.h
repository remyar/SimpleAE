#ifndef _SPARK_H_
#define _SPARK_H_

#include "./Low-Level/board.h"
#include "./bobine.h"

void SPARK_Init(void);
bool SPARK_Run(uint32_t D , uint32_t T);
#endif
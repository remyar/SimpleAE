#ifndef _AEPL_H_
#define _AEPL_H_

#include "./compc/COM_PC.h"

extern int Anga[];
extern int Ncyl;
extern int unsigned long NT;
extern int unsigned long T;
extern const int AngleCapteur;
extern int unsigned long D;
extern int tcor;
extern int AngleCibles;
extern int unsigned long Davant_rech;

void Init();
void RegRalenti();
void Genere_multi();
void Tst_Pot();
void Smartphone();
void Select_Courbe();
void Prep_Courbe();
void isr_GestionIbob();

void AEsetup();
void AEloop();

#endif
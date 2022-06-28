
//*****************************************************************************

#include "./compc/COM_PC.h"
#include "./aepl.h"

////////////////////////////////////////////////////////////////////////
void setup() ///////////////
/////////////////////////////////////////////////////////////////////////
{
    AEsetup();

    //-- init table d'avance
   /* for ( int i = 0 ; i < 16 ; i++){
        Anga[i] = MEMORY_ReadUnsignedInt(32 + (i * 2));
        if ( Anga[i] < 0 ){
            Anga[i] = 0;
        }
    }*/


    COMPC_TaskInit();

}

///////////////////////////////////////////////////////////////////////////
void loop() ////////////////
////////////////////////////////////////////////////////////////////////////
{
    AEloop();
    COMPC_TaskRun();
}


/////////////////Exemples de CAPTEURS/////////////////
//Capteur Honeywell cylindrique 1GT101DC,contient un aimant sur le coté,type non saturé, sortie haute à vide,
//et basse avec une cible en acier. Il faut  CapteurOn = 0, declenchement sur front descendant.
//Le capteur à fourche SR 17-J6 contient un aimant en face,type saturé, sortie basse à vide,
//et haute avec une cible en acier. Il faut  CapteurOn = 1, declenchement sur front montant.

//Pour les Ncyl pairs:2,4,6,8,10,12, le nombre de cibles réparties sur le vilo est Ncyl/2
//Dans les deux cas (capteur sur vilo ou dans l'alumeur) la periode entre deux cibles et la même car l'AàC tourne à Nvilo/2
//Pour les Ncyl impairs 1,3 5, 7?,9? il FAUT un capteur dans l'alumeur (ou AàC)


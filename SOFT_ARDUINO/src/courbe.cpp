#include "./courbe.h"
#include "./engine.h"

//___1___ Na[] régime moteur en t/mn
//Na[] et Anga[] doivent obligatoirement débuter puis se terminer par  0, et  contenir des valeurs  entières >=1
//Le dernier Na fixe la ligne rouge, c'est à dire la coupure de l'allumage.
//Le nombre de points est libre.L'avance est imposée à 0° entre 0 et Nplancher t/mn
//Courbe Savoy plus V au début
static float Na[16] = {0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000, 6500,7000,0}; //t/*mn vilo

//__2__Anga [] degrés d'avance vilebrequin correspondant ( peut croitre ou decroitre)
static float Anga[16] = {0, 16, 10, 16, 18, 22, 26, 28, 28, 28, 29, 30, 32, 32,32, 0};

static uint8_t idxCourbe = 0;
static float C1[30];     //Tableaux des constantes de calcul de l'avance courante
static float C2[30];     //Tableaux des constantes de calcul de l'avance courante
static float Tc[30];     //Tableau des Ti correspondants au Ni
static float tcor = 140; //correction en µs  du temps de calcul pour D
static float Tlim = 0;   //Période minimale, limite, pour la ligne rouge
static int j_lim = 0;                 //index maxi des N , donc aussi  Ang
static float Tplancher = 0;               //idem//idem
static float Nplancher = 500;               // vitesse en t/mn jusqu'a laquelle l'avance  = 0°
static float RDzero = 0;                  //pour calcul delai avance 0° < Nplancher t/mn
static float T;
static float D;

void COURBE_Init()
{
    float N1 = 0;
    float Ang1 = 0; //Toute courbe part de  0

    float AngleCibles = 720 / ENGINE_GetNbCylindres(); //Cibles sur vilo.Ex pour 4 cylindres 180°,  120° pour 6 cylindres
    float NT = (float)120000000 / (float)ENGINE_GetNbCylindres();          //Facteur de conversion Nt/mn moteur, Tµs entre deux PMH étincelle

    N1 = 0;
    Ang1 = 0; //Toute courbe part de  0
    Tplancher = (float)120000000 / (float)Nplancher / (float)ENGINE_GetNbCylindres(); //T à  vitesse plancher en t/mn: en dessous, avance centrifuge = 0
    RDzero = float(POSITION_CAPTEUR) / float(AngleCibles);

    for ( int i = 0 ; i < 16 ; i++ ){
        Na[i] = (float)MEMORY_ReadUnsignedInt(i);
        Anga[i] = (float)MEMORY_ReadUnsignedInt(32+i);
    }

    for (uint8_t i = 1; Na[i] != 0; i++) //i pour les C1,C2 et Tc
    {
        float N2 = Na[i];
        float Ang2 = Anga[i];
        float k = float(Ang2 - Ang1) / float(N2 - N1); //pente du segment (1,2)
        C1[i] = float(POSITION_CAPTEUR - Ang1 + k * N1) / float(AngleCibles);
        C2[i] = -float(NT * k) / float(AngleCibles) - tcor; //Compense la durée de calcul de D
        Tc[i] = float(NT / N2);

        N1 = N2;
        Ang1 = Ang2; //fin de ce segment, début du suivant

        j_lim = i;
        Tlim = Tc[i]; //Ligne rouge
    }
}

void COURBE_Select(uint8_t idx)
{
    idxCourbe = idx;
}

float COURBE_GetLimit(void)
{
    return (float)Tlim;
}

float COURBE_GetRpm(void){
    return T;
}

float COURBE_GetDelay(void){
    return D;
}

uint32_t COURBE_TaskRun(uint32_t _T)
{
    uint32_t _D;
    T = _T;
    for (int j = 1; j <= j_lim; j++) //On commence par T la plus longue et on remonte
    {
        if (_T >= Tc[j])
        {                                           //on a trouvé le bon segment de la courbe d'avance
            _D = float(((float)_T * C1[j]) + C2[j]); //D en µs, C2 incorpore le temps de calcul tcor
            if (_T > Tplancher)
                _D = _T * RDzero; //Imposer 0° d'avance de 0 à 500t/mn
            break;              //Sortir, on a D
        }
    }
    D = _D;
    return _D;
}
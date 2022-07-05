#include "./aepl.h"
// Aepl-Duino_23_05_22
// Allumage electronique programmable - Arduino Nano et compatibles

//*********Details sur http://loutrel.org/aeduino.php *************

//*********  Seulement  6 lignes à renseigner obligatoirement.*****
//**********Ce sont:  Na  Anga  Ncyl  AngleCapteur  CaptOn  Dwell**

//___1___ Na[] régime moteur en t/mn
// Na[] et Anga[] doivent obligatoirement débuter puis se terminer par  0, et  contenir des valeurs  entières >=1
// Le dernier Na fixe la ligne rouge, c'est à dire la coupure de l'allumage.
// Le nombre de points est libre.L'avance est imposée à 0° entre 0 et Nplancher t/mn
// Par exemple, courbe Savoy avec un " V " au début pour favoriser le ralenti
int Na[] = {0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4200, 4600, 5100, 7000, 0}; // t/*mn vilo

//__2__Anga [] degrés d'avance vilebrequin correspondant ( peut croitre ou decroitre)
int Anga[14] = {0, 16, 10, 16, 18, 22, 26, 28, 28, 28, 29, 30, 32, 0}; // Ralenti en "V"
// int Anga[] =    {0, 0 , 0 , 0  , 12,    14,  26,    28,  28,   28,   29,    30,   32,  0};
//__3__Ncyl Nombre de cylindres, moteur 4 temps.Multiplier par 2 pour moteur 2 temps
int Ncyl = 2;

//__4__Position en degrès vilo du capteur( Hall ou autre ) AVANT le PMH étincelle du cylindre N°1
const int AngleCapteur = 90; // En general autour de 45 degrès

//__5__CapteurOn = 1 déclenchement sur front MONTANT (par ex. capteur Hall "saturé")
// CapteurOn = 0 déclenchement sur front DESCENDANT (par ex. capteur Hall "non saturé").Voir fin du listing
const int CaptOn = 0;

//__6__Dwell = 1 pour alimenter la bobine en permanence sauf 1ms/cycle.Elle doit pouvoir chauffer sans crainte
// Dwell = 2 pour alimentation de la bobine seulement trech ms par cycle, 3ms par exemple
// OBLIGATOIRE pour bobine 'electronique'   de faible resistance: entre 2 et 0.5ohm.Ajuster  trech. Sinon elle brule!
// Dwell = 3 pour simuler un allumage à vis platinées: bobine alimentée 2/3 (66%) du cycle.Option à prendre par défaut
// Dwell = 4 pour optimiser l'étincelle à haut régime.La bobine chauffe un peu plus.
const int Dwell = 2;

//***************************************************************
//**********************GENERALITES *****************************

// Valable pour tout type de capteur soit sur vilo soit dans l'allumeur (ou sur l'arbre à came)
// La Led(D13) existant sur tout Arduino suit le courant dans la bobine:ON bobine alimentée
// Pour les moteurs à 1, 3 ou 5 cylindres, 4 temps, il FAUT un capteur dans l'allumeur
// ou sur l'arbre à cames, c'est la même chose.
// Exception possible pour un monocylindre 4 temps, avec  capteur sur vilo et une cible:on genere
// une étincelle perdue au Point Mort Bas en utilisant la valeur Ncyl =2.
// Avance 0°jusqu'a Nplancher t/mn, anti retour de kick.

// En option, regulation  du ralenti par modification dynamique de l'avance
// En option, multi-étincelles à bas régime pour denoyer les bougies
// En option, variantes de Dwell
// En option, compte-tours précis
// En option, multi courbes , 2 courbes supplementaires, b et c, selectionables par D8 ou D9, ou smartphone
// En option, decalage de courbe 'au vol'
// Pour N cylindres,2,4,6,8,12,16, 4 temps, on a N cames d'allumeur ou  N/2 cibles sur le vilo

//**********************LES OPTIONS**********************

//*************Régulation du ralenti par modification dynamique de l'avance
//* Sur une idée d'Antoine Zorgati
//* L'avance au ralenti est totalement indépendante de la courbe   d'allumage
//* On fixe une avance de départ AvR = 10 degrès et une zone de consigne de ralenti avec une tolérance, par exemple 800+-50tr/min
//* Si le régime est hors consigne, on corrige l'avance de 1 degrè en plus ou en moins.
//* On conserve cette avance pendant un nombre donné CtCyInit de cycles moteurs pour stabiliser le régime
//* Puis on vérifie de nouveau le régime: si à l'intérieur de la consigne, on ne touche pas à l'avance, si hors consigne on la corrige d'un point
//* Ainsi de suite dans la zone de ralenti définie par NrMin et NrMax...

const int Nr = 0; // Consigne de ralenti régulé en t/mn,800 par exemple. Si 0 pas de regulation.

//***********************Multi-éticelles******************
// Si multi-étincelle désiré jusqu'à N_multi, modifier ces deux lignes
const int Multi = 0;      // 1 pour multi-étincelles, 0 sinon
const int N_multi = 1300; // 1300 t/mn pour 4 cylindres par exemple
// Surtout utile pour eviter de noyer les bougies à bas régime

//*************Variantes de Dwell **************
// Si Dwell=2, temps de recharge bobine, 3ms= 3000µs typique, 7ms certaines motos
const int trech = 3000;
// Si Dwell=4, durée de l'étincelle tetin au delà de Ntrans
const int Ntrans = 3000; // Regime de transition et maxi affichage RPM Sphone
const int tetin = 500;   // Typique 500 à 1000µs, durée etincelle regimes >Ntrans

//*********************Compte-tours précis************************
// Connecter un HC05/06 Au +5V, masse, patte RX à la patte 11 de l'Arduino, TX à patte 10
// IMPORTANT: mettre le HC05/06 en mode 115200 bps via une commande AT
// http://www.loutrel.org/BlueToothArduino.html pour effectuer ce passage
// Sur le smartphone installer une appli telle que "Bluetooth Terminal HC-05"
// ou encore "BlueTerm+" ou equivallent.La première fois seulement:
// inscrire le module sur le smartphone avec le code pin 1234..
// Ceci affiche sur un smartphone(ou tablette)Android  le régime (t/mn /10) et l'avance en degrès
// Surtout utile comme compte-tours precis pour regler la carburation au ralenti
#define Naff 20 // Pour affichage sur smartphone nb de tours à ignorer entre deux affichages
// Maxi 3000t/mn

//*******************MULTICOURBES**************************
// Mettre D8 ou D9 à la masse pour selectionner la courbe b ou c
// ou utiliser un smartphone et un module Bluetooth

//**********Courbe   b  mettre D8 à la masse ou entrer 8 au Smartphone
// Courbe Hervé
int Nb[] = {0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4200, 4600, 5100, 7000, 0}; // t/*mn vilo
int Angb[] = {0, 8, 10, 22, 24, 26, 28, 30, 30, 30, 32, 32, 32, 0};

//*********Courbe   c  mettre D9 à la masse ou entrer 9 au Smartphone
// C'est la courbe Savoy originale
int Nc[] = {0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4200, 4600, 5100, 7000, 0}; // t/*mn vilo
int Angc[] = {0, 6, 10, 16, 18, 22, 26, 28, 28, 28, 29, 30, 32, 0};

//*******************Decalage ou changement de courbe 'au vol'********
// Pour jouer à decaler les courbes d'avance moteur en fonctionnement
// Avec un module Bluetooth HC05 ou 06
// Entrer 1 à 6 pour decaler la courbe en cours de 1 à +6°
// Entrer 11 à 16 pour decaler la courbe en cours de -1 à -6°
// Pour changer de courbe, entrer 8 pour basculer sur la courbe b, 9 pour  la c, et 7 pour la a
// Attention.....Pas de pleine charge avec trop d'avance, danger pour les pistons!!!
// A défaut de Bluetooth, connecter un potar de 100k sur A0, ajuster le pas de decalage en degrés dans delPot
// Potentiomètre 100k de decalage à 3 positions:Decalage 0degrès  si <1.5V
// Ajoute delPot degrès  si environ 2 V, 2*delPot degrès si envron 3V

//************Valeurs sont eventuellement modifiables*****************
// Ce sont: Nplancher, Dsecu
const int Nplancher = 500;               // vitesse en t/mn jusqu'a laquelle l'avance  = 0°
const int unsigned long Dsecu = 1000000; // Securite: bobine coupee à l'arret apres Dsecu µs

//***********************LOGICIEL*****************************
#include "TimerOne.h"
#include <SoftwareSerial.h>
// Pour module Bluetooth HC05ou 06
//SoftwareSerial BT(7, 6); // Creer une entrée/sortie série par logiciel sur D7(RX sur HC) et D6(TX sur HC)
//***********************Variables du sketch************************************
#define Bob 4            // Sortie D4 vers bobine.En option, on peut connecter une Led avec R=330ohms vers la masse
#define Cible 2          // Entrée sur D2 du capteur, avec R PullUp
#define Pot A0           // Entrée analogique sur A0 pour potar de changement de courbes, avec R PullUp
#define Led13 13         // Temoin sur tout Arduino, suit le courant de bobine
#define Courbe_b 8       // Entré D8  R PullUp.Connecter à la masse pour courbe b
#define Courbe_c 9       // Entré D9  R PullUp. Connecter à la masse pour courbe c
int valPot = 0;          // 0 à 1023 selon la position du potentiomètre en entree
int delPot = 2;          // le pas de decalage en degrès
float modC1 = 0;         // Correctif pour C1[], deplace la courbe si potar connecté
int unsigned long D = 0; // Delai en µs à attendre après passage de la cible pour l'étincelle
int milli_delay = 0;
int micro_delay = 0;
float RDzero = 0;                  // pour calcul delai avance 0° < Nplancher t/mn
float Tplancher = 0;               // idem
int tcor = 140;                    // correction en µs  du temps de calcul pour D
int unsigned long Davant_rech = 0; // Delai en µs avant la recharge de la  bobine.
int unsigned long prec_H = 0;      // Heure du front precedent en µs
int unsigned long T = 0;           // Periode en cours
int unsigned long Tprec = 0;       // Periode precedant la T en cours, pour calcul de Drech
int N1 = 0;                        // Couple N,Ang de debut d'un segment
int Ang1 = 0;                      // Angle d'avance vilo en degrès
int N2 = 0;                        // Couple N,Ang de fin de segment
int Ang2 = 0;
int *pN = &Na[0];   // pointeur au tableau des régimes. Na sera la courbe par defaut
int *pA = &Anga[0]; // pointeur au tableau des avances. Anga sera la  courbe par defaut
float k = 0;        // Constante pour calcul de C1 et C2
float C1[30];       // Tableaux des constantes de calcul de l'avance courante
float C2[30];       // Tableaux des constantes de calcul de l'avance courante
float Tc[30];       // Tableau des Ti correspondants au Ni
// Si necessaire, augmenter ces 3 valeurs:Ex C1[40],C2[40],Tc[40]
int Tlim = 0;                  // Période minimale, limite, pour la ligne rouge
int j_lim = 0;                 // index maxi des N , donc aussi  Ang
int unsigned long NT = 0;      // Facteur de conversion entre N et T à Ncyl donné
int unsigned long NTa = 0;     // Facteur de conversion entre N et T pour affichage sur smartphone
int ctNaff = 0;                // Compteur de Naff
int AngleCibles = 0;           // Angle entre 2 cibles, 180° pour 4 cyl, 120° pour 6 cyl, par exemple
int UneEtin = 1;               //=1 pour chaque étincelle, testé et remis à zero par isr_GestionIbob()
int Ndem = 60;                 // Vitesse estimée du vilo entrainé par le demarreur en t/mn
int unsigned long Tdem = 0;    // Periode correspondante à Ndem,forcée pour le premier tour
int Mot_OFF = 0;               // Sera 1 si moteur detecté arrété par l'isr_GestionIbob()
int unsigned long Ttrans;      // T transition de Dwell 4 et aussi seuil affichage Sphone
int unsigned long T_multi = 0; // Periode minimale pour multi-étincelle
// Permet d'identifier le premier front et forcer T=Tdem, ainsi que Ibob=1, pour demarrer au premier front
String Ligne;  // Stock une ligne du smartphone,ex "5" pour augmenter l'avance de 5°
char carLu;    // Sera cumulé dans Ligne
int delAv = 0; // Demandé au clavier du Sphone, le decalage d'avance
// choix de courbe demandé au Sphone: 7 courbe a, 8 courbe b, 9 courbe c
int Ncourbe = 7; // Numero de la courbe en cours à afficher, au demarrage c'est 7 donc courbe a
int NrMin = 600; // la zone de ralenti est de NrMin à NrMax
int NrMax = 1200;
int unsigned long TrMax = 0; // Pour s'ejecter du ralenti  rapidement
int Nrinf = 750;             // La zone de regulation est de Nrinf à Nrsup
int Nrsup = 850;
int CtCy = 0;      // Compteur de cycles/periodes entre deux mesures de ralenti régulé
int CtCyInit = 20; // Soit CtCyInit/2 tours moteurs sur un 4 cylindres
int AvR = 10;      // Avance ajustée dynamiquement pour le ralenti régulé
int AvRMin = 1;    // Bornes pour l'avance au ralenti régulé
int AvRMax = 25;   // 20 ou 25 ou 30 degrès va aussi
int N = 0;         // En t/mn pour tests de ralenti régulé

void AEsetup() ///////////////
/////////////////////////////////////////////////////////////////////////
{
    Serial.begin(115200); // Ligne suivante, 3 Macros du langage C
                          // Serial.println(__FILE__);
                          //  Serial.println(__DATE__);
                          //  Serial.println(__TIME__);
    //BT.begin(115200);     // Vers module BlueTooth HC05/06
   // BT.flush();           // A tout hasard
   // BT.println(__FILE__);
   // BT.println(__DATE__);
   // BT.println(__TIME__);
   // BT.println("***Bonjour*****************************************");
   // BT.println("******************************************************");

/*
Serial.print("AT");
delay(1000);
while(Serial.available()){
    Serial.print(Serial.read());
}
Serial.print("AT+BAUD8");
delay(1000);
while(Serial.available()){
    Serial.print(Serial.read());
}


    while(true){}
 */   


    pinMode(Cible, INPUT_PULLUP);    // Entrée front du capteur sur D2
    pinMode(Bob, OUTPUT);            // Sortie sur D4 controle du courant dans la bobine
    pinMode(Pot, INPUT_PULLUP);      // Entrée pour potard 10kohms, optionnel
    pinMode(Courbe_b, INPUT_PULLUP); // Entrée à la masse pour selectionner la courbe b
    pinMode(Courbe_c, INPUT_PULLUP); // Entrée à la masse pour selectionner la courbe c
    pinMode(Led13, OUTPUT);          // Led d'origine sur tout Arduino, temoin du courant dans la bobine
    // Tst_BT();  //Option pour tester le Bluetooth, boucle ici,  sinon à commenter.
    Init(); // Executée une fois au demarrage
}
///////////////////////////////////////////////////////////////////////////

//********************LES FONCTIONS*************************

void CalcD() //////////////////
// Noter que T1>T2>T3...
{
    for (int j = 1; j <= j_lim; j++) // On commence par T la plus longue et on remonte
    {
        if (T >= Tc[j])
        {                                           // on a trouvé le bon segment de la courbe d'avance
            D = float(T * (C1[j] - modC1) + C2[j]); // D en µs, C2 incorpore le temps de calcul tcor,modC1 decalage via Sphone
            if (T > Tplancher)
                D = T * RDzero; // Imposer 0° d'avance de 0 à 500t/mn
            break;              // Sortir, on a D
        }
    }
}
void CalcD_Test() ///////////////////////////////////////
{
    if (T < TrMax)
        CalcD(); // Hors ralenti
    else
    {
        if (Nr == 0)
            CalcD(); // En ralenti mais pas de regul
        else
            RegRalenti();
    }
}
void Etincelle() //////////
{
    if (D < 14000)
    {                         // 16383 µs semble être le maxi pour la fonction  delayMicroseconds(D)
        delayMicroseconds(D); // Attendre D }
    }
    else
    {
        milli_delay = ((D / 1000) - 2); // Pour ces D longs, delayMicroseconds(D)ne va plus.
        micro_delay = (D - (milli_delay * 1000));
        delay(milli_delay); //
        delayMicroseconds(micro_delay);
    }
    digitalWrite(Bob, 0);   // Couper le courant, donc étincelle
    digitalWrite(Led13, 0); // Temoin
    // Maintenant que l'étincelle est émise, il faut rétablir Ibob au bon moment

    if (Dwell != 2 && Multi && (T >= T_multi))
        Genere_multi(); // Sauf si Dwell=2,voir si multi étincelle requise
    else
    {
        switch (Dwell) // Attente courant coupé selon le type de Dwell

        {
        case 1:                 // Ibob coupe 1ms par cycle seulement, la bobine doit supporter de chauffer
            Davant_rech = 1000; // 1ms off par cycle
            break;

        case 2:                                  // Type bobine faible resistance, dite "electronique"
            Davant_rech = 2 * T - Tprec - trech; // On doit tenir compte des variations de régime moteur
            Tprec = T;                           // Maj de la future periode precedente
            break;

        case 3: // Type "vis platinées", Off 1/3, On 2/3
            Davant_rech = T / 3;
            break;

        case 4: // Type optimisé haut régime
            if (T > Ttrans)
                Davant_rech = T / 3; // En dessous de N trans, typique 3000t/mn
            else
                Davant_rech = tetin; // Au delà de Ntrans, on limite la durée de l'étincelle, typique 0.5ms
            break;
        }
        Timer1.initialize(Davant_rech); // Attendre Drech µs avant de retablire le courant dans la bobine
    }
    UneEtin = 1;  // Pour signaler que le moteur tourne à l'isr_GestionIbob().
    Tst_Pot();    // Voir si un potar connecté pour decaler la courbe
                  //   if (T > Ttrans)
//    Smartphone(); // Si pas trop vite gérer le sphone
COMPC_TaskRun();
}
void Genere_multi()       //////////
{                         // L'etincelle principale a juste été générée
    delay(1);             // Attendre fin d'etincelle 1ms
    digitalWrite(Bob, 1); // Retablir  le courant
    delay(3);             // Recharger 3ms
    digitalWrite(Bob, 0); // Première etincelle secondaire
    delay(1);             // Attendre fin d'etincelle 1ms
    digitalWrite(Bob, 1); // Retablir  le courant
    delay(2);             // Recharger 2 ms
    digitalWrite(Bob, 0); // Deuxième etincelle secondaire
    delay(1);             // Attendre fin d'etincelle 1ms
    digitalWrite(Bob, 1); // Retablir  le courant pour étincelle principale
}
void Init() /////////////
// Calcul de 3 tableaux,C1,C2 et Tc qui serviront à calculer D, temps d'attente
// entre la detection d'une cible par le capteur  et la generation de l'etincelle.
// Le couple C1,C2 est determiné par la periode T entre deux cibles, correspondant au
// bon segment de la courbe d'avance entrée par l'utilisateur: T est comparée à Tc
{
    AngleCibles = 720 / Ncyl; // Cibles sur vilo.Ex pour 4 cylindres 180°,  120° pour 6 cylindres
    NT = 120000000 / Ncyl;    // Facteur de conversion Nt/mn moteur, Tµs entre deux PMH étincelle
    // c'est à dire deux cibles sur vilo ou deux cames d'allumeur: Nt/mn = NT/Tµs
    NTa = NT / 10;                            /// Facteur de conversion Nt/mn moteur pour afficher N/10 au smartphone
    TrMax = NT / NrMax;                       // Pour ralenti régulé
    Ttrans = NT / Ntrans;                     // Calcul de la periode de transition pour Dwell 4 et affichage Sphone
    T_multi = NT / N_multi;                   // Periode minimale pour generer un train d'étincelle
    modC1 = 0;                                // Pas de modif d'avance
    Tdem = NT / Ndem;                         // Periode imposée à  la première étincelle qui n'a pas de valeur prec_H
    Tplancher = 120000000 / Nplancher / Ncyl; // T à  vitesse plancher en t/mn: en dessous, avance centrifuge = 0
    RDzero = float(AngleCapteur) / float(AngleCibles);
    Prep_Courbe(); // Calcul les segments de la courbe d'avance specifiée par Ncourbe
    //  Serial.print("Ligne_"); Serial.println(__LINE__);
    //  Serial.print("Tc = "); for (i = 1 ; i < 15; i++)Serial.println(Tc[i]);
    //  Serial.print("Tlim = "); Serial.println(Tlim);
    //  Serial.print("C1 = "); for (i = 1 ; i < 15; i++)Serial.println(C1[i]);
    //  Serial.print("C2 = "); for (i = 1 ; i < 15; i++)Serial.println(C2[i]);
    // Timer1 a deux roles:
    // 1)couper le courant dans la bobine en l'absence d'etincelle pendant plus de Dsecu µs
    // 2)après une étincelle, attendre le delais Drech avant de retablir le courant dans la bobine
    // Ce courant n'est retabli que trech ms avant la prochaine étincelle, condition indispensable
    // pour une bobine à faible resistance, disons inférieure à 3 ohms.Typiquement trech = 3ms à 7ms
    Timer1.attachInterrupt(isr_GestionIbob); // IT d'overflow de Timer1 (16 bits)
    Timer1.initialize(Dsecu);                // Le courant dans la bobine sera coupé si aucune etincelle durant Dsecu µs
    Mot_OFF = 1;                             // Signalera à loop() le premier front
    digitalWrite(Bob, 0);                    // par principe, couper la bobine
    digitalWrite(Led13, 0);                  // Temoin
}
void isr_GestionIbob() //////////
{
    Timer1.stop(); // Arreter le decompte du timer
    if (UneEtin == 1)
    {
        digitalWrite(Bob, 1);   // Le moteur tourne,retablire le courant dans bobine
        digitalWrite(Led13, 1); // Temoin
    }
    else
    {
        digitalWrite(Bob, 0);
        digitalWrite(Led13, 0); // Temoin//Moteur arrete, preserver la bobine, couper le courant
        Mot_OFF = 1;            // Permettra à loop() de detecter le premier front de capteur
    }
    UneEtin = 0;              // Remet  le detecteur d'étincelle à 0
    Timer1.initialize(Dsecu); // Au cas où le moteur s'arrete, couper la bobine apres Dsecu µs
}
void Lect_delAv() ///////////////////Uniquement si T> Ttrans, typique N <3000t/mn
// Lecture du smartphone
{                              // Avec 7,8 ou 9 on change de courbe, sinon delta de +1 deg à +6 deg  ou negatif 11 à 16, 0 pour courbe non modifiée
    Ligne = "";                // On y accumule les car reçus du sphone
    while (Serial.available() > 0) // Nb de car dans le buffer venant du sphone
    {
        carLu = Serial.read(); // oui il y a un car au moins, mais on filtre 1 à 9 seulement, on se méfie
        if ((carLu >= 48) && (carLu <= 57))
            Ligne = Ligne + carLu; // si entre 0 et 9
    }
    //  Serial.println( Ligne);
    if (Ligne != "") // Une valeur a été entrée au sphone
    {
        delAv = Ligne.toInt(); // convertir en entier
        //    Serial.println( delAv);
        if ((delAv == 8) || (delAv == 9) || (delAv == 7))
        {
            {
                Ncourbe = delAv; // Sauver pour affichage
                Prep_Courbe();   // On change de courbe, appel Select_Courbe()
            }
        }
        else // Delta sur courbe en cours de +1 a +6 deg ou negatif
        {
            if ((delAv >= 0) && (delAv <= 16))
            {
                if (delAv >= 11)
                    delAv = 10.0 - delAv;                  // par ex  on entre 12, pour -2°
                modC1 = float(delAv) / float(AngleCibles); // pour le calcul du delai avant étincelle
                Serial.println(delAv);                         // Afficher sur sphone
            }
            else
                delAv = 0;
        }
    }
}
void Prep_Courbe()   /////////Calcul les segments de la courbe d'avance
{                    // Ncourbe =  7,8,ou 9 pour la courbe à activer, a ou b ou c
    modC1 = 0;       // Pas de correction
    Select_Courbe(); // Ajuster  les pointeurs pN et pA pour la courbe designée par Ncourbe
    delAv = 0;       // Raz du decalage
    N1 = 0;
    Ang1 = 0;  // Toute courbe part de  0
    int i = 0; // locale mais valable hors du FOR
    pN++;
    pA++;                      // sauter le premier element de tableau, toujours =0
    for (i = 1; *pN != 0; i++) // i pour les C1,C2 et Tc.Arret quand regime=0.
                               // pN est une adresse (pointeur) qui pointe au tableau N.Le contenu pointé est *pN
    {
        N2 = *pN;
        Ang2 = *pA;                              // recopier les valeurs pointées dans N2 et Ang2
        k = float(Ang2 - Ang1) / float(N2 - N1); // pente du segment (1,2)
        C1[i] = float(AngleCapteur - Ang1 + k * N1) / float(AngleCibles);
        C2[i] = -float(NT * k) / float(AngleCibles) - tcor; // Compense la durée de calcul de D
        Tc[i] = float(NT / N2);                             //
        N1 = N2;
        Ang1 = Ang2; // fin de ce segment, début du suivant
        pN++;
        pA++; // Pointer à l'element suivant de chaque tableau
    }
    j_lim = i - 1;    // Revenir au dernier couple entré
    Tlim = Tc[j_lim]; // Ligne rouge
}
void RegRalenti()                   ////////////////////////////////////
{                                   // On va calculer un D specifique pour le ralenti régulé, ignorant les courbes d'avance
    CtCy++;                         // Compteur de passage, on ne teste que tous les CtCyInit passages
    if (CtCy >= CtCyInit)           // Sinon,conserver AvR pour calculer D
    {                               // Verifier AvR
        CtCy = CtCyInit;            // Re init le compteur de passages
        N = NT / T;                 // Calculer N pour les tests à suivre
        if (N < Nrinf || N > Nrsup) // Sinon c'est bon  pas de correction, garder AvR
        {                           // correction necessaire pour AvR
            if (N < Nrinf)
            {
                AvR++; // Trop lent
                if (AvR > AvRMax)
                    AvR = AvRMax; // Plafonner haut
            }
            else
            {
                AvR--; // Trop vite
                if (AvR < AvRMin)
                    AvR = AvRMin; // Plafonner bas
            }
        }
    }
    D = (T / AngleCibles) * (AngleCapteur - AvR); // On neglige les corrections car au ralenti
}
void Select_Courbe() ///////////
// Initialise pN et pA vers  une des 3 courbes a , b , c selon Ncourbe
{ // Serial.println("********************************Ncourbe**********");
    // Serial.println( Ncourbe);
    if (Ncourbe == 7) // C'est la  courbe d'origine par défaut, a
    {
        pN = &Na[0];
        pA = &Anga[0];
    }
    if ((digitalRead(Courbe_b) == 0) || (Ncourbe == 8)) // D8 à la masse ou 8 au Sphone
    {
        pN = &Nb[0]; // pointer à la courbe b
        pA = &Angb[0];
    }
    if ((digitalRead(Courbe_c) == 0) || (Ncourbe == 9)) // D9 à la masse ou 9 au Sphone
    {
        pN = &Nc[0]; // pointer à la courbe c
        pA = &Angc[0];
    }
}

void Smartphone() ///////////////////////////////////////////
{                 // Si  N < Ntrans on affiche en Bluetooth le regime, l'avance et possibilité de decaler ou changer de courbe
    static long ms = millis();

    if ((millis() - ms) > 50)
    { // Afficher vitesse, avance et N° de courbe

        Serial.print("[IV:");
        Serial.print(NT / T); // Afficher N et avance sur smartphone
        Serial.print(":");
        Serial.print((float)((float)AngleCapteur - (float)((D + tcor) * (float)AngleCibles) / (float)T)); // Afficher avance
        Serial.print(":");
        Serial.print(Davant_rech);
        Serial.print("]");
        /*
        Serial.print(NT / T); // Afficher N et avance sur smartphone
        Serial.print("................deg  ");
        Serial.println(AngleCapteur - (D + tcor) * AngleCibles / T); // Afficher avance
        Serial.print("Courbe ");
        Serial.println(Ncourbe); // Afficher 7 ou 8 ou 9  pour la courbe en cours
        Serial.print("Decalage ");
        Serial.println(delAv);
        Serial.println();*/
        ms = millis(); // RAZ du compteur d'affichages
        Lect_delAv();  // Voir si decalage ou basculement de courbe demandée au sphone
    }
}
void Tst_BT()
{
    int BTdata = 0;
    //**************Vers PC
    Serial.println("Bonjour ");
    Serial.println("Le caractere entre sur l'Android doit se retrouver ci dessous");
    //***************Vers module
    Serial.println(" ");
    Serial.println(" ");
    Serial.println("Bonjour sur Android");
    Serial.println(" ");
    Serial.println("HC05, la led doit clignoter 2 flash, 1s Off, 2flash...");
    Serial.println(" ");
    Serial.println("HC06, led On fixe");
    Serial.println("");
    Serial.println("Entrer un caractere ");
    Serial.println("S'il se retouve sur le PC, tout va bien...");
    while (1)
    {
        if (Serial.available()) // Car pret en entrée sur Serie soft?
        {
            BTdata = Serial.read(); // oui, le clavier Android BT a émis un car
            Serial.println(" ");    // Aller à la ligne sur sphone

            // **************Vers PC
            Serial.write(BTdata); // ecrire le car sur le PC
            Serial.println();
        }
    }
}
void Tst_Pot() ///////////
{
    valPot = analogRead(Pot); // Potar 100k >1.
    if (valPot < 900)         // Pas de potar connecté (valpot =1023 en théorie)
    {                         // Potar connecté, correction 0 ou delpot degrés ou 2*delPot degrès
        if (valPot < 240)
            modC1 = 0; // Pas de correction, environ <1.5V
        else if (valPot < 700)
            modC1 = float(delPot) / float(AngleCibles); // Ajouter delPot degrès, environ 2V
        else
            modC1 = 2 * float(delPot) / float(AngleCibles); // Ajouter 2*delPot degrès, environ 3V
    }
    //  Serial.println(valPot); Serial.println(modC1);
}
////////////////////////////////////////////////////////////////////////
void AEloop() ////////////////
////////////////////////////////////////////////////////////////////////////
{
    long _ms = millis();
    while (digitalRead(Cible) == !CaptOn) // Attendre front actif de la cible
    {
        if ( Mot_OFF == 1 ){
            if ( (millis() - _ms) >= 250 ){
                return;
            }
        }
    }

    T = micros() - prec_H; // front actif, arrivé calculer T
    prec_H = micros();     // heure du front actuel qui deviendra le front precedent
    if (Mot_OFF == 1)
    {                           // Demarrage:premier front de capteur
        T = Tdem;               // Fournir  T = Tdem car prec_H n'existe par pour la première étincelle
        digitalWrite(Bob, 1);   // Alimenter la bobine
        digitalWrite(Led13, 1); // Temoin
        Mot_OFF = 0;            // Le moteur tourne
    }
    if (T > Tlim) // Sous la ligne rouge?
    {
        CalcD_Test(); // Oui, generer une etincelle
        Etincelle();
    }
    while (digitalRead(Cible) == CaptOn)
        ; // Attendre si la cible encore active
}
/////////////////Exemples de CAPTEURS/////////////////
// Capteur Honeywell cylindrique 1GT101DC,contient un aimant sur le coté,type non saturé, sortie haute à vide,
// et basse avec une cible en acier. Il faut  CapteurOn = 0, declenchement sur front descendant.
// Le capteur à fourche SR 17-J6 contient un aimant en face,type saturé, sortie basse à vide,
// et haute avec une cible en acier. Il faut  CapteurOn = 1, declenchement sur front montant.

// Pour les Ncyl pairs:2,4,6,8,10,12, le nombre de cibles réparties sur le vilo est Ncyl/2
// Dans les deux cas (capteur sur vilo ou dans l'alumeur) la periode entre deux cibles et la même car l'AàC tourne à Nvilo/2
// Pour les Ncyl impairs 1,3 5, 7?,9? il FAUT un capteur dans l'alumeur (ou AàC)

////////////////DEBUGGING////////////////////////
// Voir   http://www.loutrel.org/Arduinodebugging.php
// Macro  ps(v) de debug pour imprimer le numero de ligne, le nom d'une variable, sa valeur
// puis s'arreter definitivement
//#define ps(v) Serial.print("Ligne_") ; Serial.print(__LINE__) ; Serial.print(#v) ; Serial.print(" = ") ;Serial.println((v)) ; Serial.println("  Sketch stop"); while (1);
// Exemple, à la ligne 140, l'instruction     ps(var1);
// inprimera  "Ligne_140var1 = 18  Sketch stop"
// Macro  pc(v)de debug pour imprimer le numero de ligne, le nom d'une variable, sa valeur,
// puis s'arreter et attendre un clic de souris  sur le bouton 'Envoyer'en haut de l'ecran seriel pour continuer.
#define pc(v)                                                \
    Serial.print("Ligne_");                                  \
    Serial.print(__LINE__);                                  \
    Serial.print(#v);                                        \
    Serial.print(" = ");                                     \
    Serial.println((v));                                     \
    Serial.println(" Clic bouton 'Envoyer' pour continuer"); \
    while (Serial.available() == 0)                          \
        ;                                                    \
    {                                                        \
        int k_ = Serial.parseInt();                          \
    }
// Exemple, à la ligne 145, l'instruction    pc(var2);
//  inprimera   "Ligne_145var2 = 25.3   Clic bouton 'Envoyer' pour continuer"
// float gf = 0;//pour boucle d'attente,gf  GLOBALE et FLOAT indispensable
//   gf = 1; while (gf < 2000)gf++;//10= 100µs,100=1.1ms,2000=21.8ms
// void  Top()//////////
//{ digitalWrite(Bob, 1); //Crée un top sur l'oscillo
//   gf = 1; while (gf < 10)gf++;//gf DOIT être Globale et Float 10=100µs,2000=21.8ms, retard/Cible=50µs
//   digitalWrite(Bob, 0); //
// }
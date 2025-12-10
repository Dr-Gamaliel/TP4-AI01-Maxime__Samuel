#ifndef TP4_H_INCLUDED
#define TP4_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <windows.h>
//#include <conio.h>
//#include <wincon.h>
#define KBUP 328
#define KBDOWN 336

/// déclaration de la structure
typedef struct _Position
{
    int numeroLigne;
    int ordre;
    int numeroPhrase;
    struct _Position* suivant;
}T_Position;
typedef struct _Noeud
{
    char* mot;
    int nbOccurences;
    T_Position* listePositions;
    struct _Noeud* filsGauche;
    struct _Noeud* filsDroit;
}T_Noeud;
typedef struct _Index
{
    T_Noeud* racine;
    int nbMotsDistincts;
    int nbMotsTotal;
}T_Index;

///déclaration du menu
/*void color(int t,int f);
void gotoxy(int x, int y);*/
void initialisation(char** LM);
int menu(char** tab);//, COORD O);

///déclaration des fonctions
T_Position* ajouterPosition(T_Position* listeP, int ligne, int ordre, int phrase);
#endif // TP4_H_INCLUDED

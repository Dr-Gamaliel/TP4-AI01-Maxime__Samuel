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
#define HORIZONTALE 196
#define VERTICALE 195
#define TABLEAU_GAUCHE_HAUT 218
#define TABLEAU_GAUCHE_BAS 192

/// déclaration des structures
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

/// on crée une structure file pour le stockage des éléments de l'arbre en vue de leur traitement à la fin du parcours
typedef struct _File
{
    T_Noeud* tab;
    int head;
    int tail;
    int Longueur;
}File;
// on défini les fonctions élémentaires de la file
File* creer_file(int l);//O(1)
int est_vide(File* F);//O(1)
int est_pleine(File* F);//O(1)
void enfiler(File* F, const T_Noeud* n);//O(1)
const T_Noeud* defiler(File* F);//O(1)


///déclaration du menu
/*void color(int t,int f); //O(1)
void gotoxy(int x, int y);//O(1) */
void initialisation(char** LM);//O(1)
int menu(char** tab);//, COORD O);//O(1)

///déclaration des fonctions
T_Position* ajouterPosition(T_Position* listeP, int ligne, int ordre, int phrase,int* maj); //O(n)(n= nombre d'occurences du mot)
T_Noeud* rechercherMot(T_Index* index, char *mot);//O(n)
void test();//cette fonction est utilisée pour tester nos programmes en cours de devellopement
void afficher_noeud(const T_Noeud* X);//cette fonction permet d'afficher l'information située à un noeud
void parcours_infixe(T_Noeud* X, File* F);//cette fonction permet de parcourir l'arbre par ordre croissant// O(m)
void afficherIndex(T_Index* index);//cette fonction permet d'afficher les elements par ordre alphabétique//O(m*n)
#endif // TP4_H_INCLUDED

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

/// déclaration des structures
typedef struct _Position
{
    int numeroLigne;
    int ordreLigne;
    int numeroPhrase;
    int ordrePhrase;
    char* memory;
    struct _Position* suivant;
}T_Position;
typedef struct _Noeud
{
    char* mot;
    int nbOccurences;
    T_Position* listePositions;
    struct _Noeud* filsGauche;
    struct _Noeud* filsDroit;
    int hauteur;
}T_Noeud;
typedef struct _Index
{
    T_Noeud* racine;
    int nbMotsDistincts;
    int nbMotsTotal;
    int nbPoints;
    char *nomFichier;
}T_Index;

/// structure et fonction pour l'affichage des occurences d'un mot
///-------structures------
typedef struct _celluleMot
{
    char* mot;
}celluleMot;

typedef struct _Pile
{
    celluleMot* tab;
    int* ligne;
    int sommet;
    int Longueur;
}Pile;

typedef struct _element
{
    int numphrase;
    Pile* phrase;
}Element;
///-------fonctions ------
Pile* creer_pile(int l);//O(n)//au pire des cas, le texte tout entier ne contient qu'une phrase, celle que l'on veut recréer
void supprimer_Pile(Pile* P);//O(1)
int est_vide(Pile* P);//O(1)
int est_pleine(Pile* P);//O(1)
void empiler(Pile* P, char* mot, int ordre,int ligne);//O(1)
char* depiler(Pile* P);//O(1)
int dicho_search(Element* tab, int X, int debut, int fin);//O(log(n))
void parcours_position(T_Position* X, Element* E, int taille_E);//O(n)
void parcours_infixe(T_Noeud* X, Element* E, int taille_E);//O(n)


/*typedef struct CellulePhrase {
    int numeroPhrase;
    T_CelluleMot *listeMots;
    struct CellulePhrase *suivant;
} T_CellulePhrase;*/

///déclaration du menu
/*void color(int t,int f);//O(1)
void gotoxy(int x, int y);//O(1)*/
void initialisation(char** LM);//O(1)
int menu(char** tab);//, COORD O);//O(1)

///déclarations des fonctions secondaires
int maxi(int a, int b);//O(1)
int hauteur(T_Noeud *N);//O(1)
int getEquilibre(T_Noeud *N);//O(1)
T_Noeud *rotationDroite(T_Noeud *y);//O(1)
T_Noeud *rotationGauche(T_Noeud *x);//O(1)
T_Noeud* insererAVL(T_Noeud* node, char* mot_traite, char* mot_init, int ligne, int ordreL, int phrase, int ordreP, int* estNouveau);//O(n)//où n est le nombre de mots au total dans le text
void afficherArbreRec(T_Noeud *noeud, char *derniereLettre);//O(n)//car on parcours tous les mots du textes, pas seuleument les mots distincs


///déclaration des fonctions principales
T_Position* ajouterPosition(T_Position* listeP, char* mot,int ligne, int ordreL, int phrase, int ordreP, int* maj);//O(k)//où k est le max(nboccurences)
int indexerFichier(T_Index *index,T_Position** Point, char *filename);//O(n^2)//car en principe dans une phrase on a plus de points que de mots
int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordreL, int phrase, int ordreP);//O(n)//où n est le nombre de mots au total dans le texte
T_Noeud* rechercherMot(T_Index* index, char *mot);//O(m) //où m est le nombre de mots distincs du texte, car on ne parcours que les noeuds de l'AVL
void afficherIndex(T_Index index);//O(m)// elle ne fait qu'utiliser la fonction afficherArbreRec
void afficherOccurencesMot(T_Index* index, T_Position* Point, char *mot);//O(n)//voir implémentation dans tp4_out.c pour les explications
///fonction en attente
void construireTexte(T_Index index, char *filename);

#endif

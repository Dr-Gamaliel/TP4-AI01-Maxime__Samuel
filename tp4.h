#ifndef TP4_H_INCLUDED
#define TP4_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <conio.h>
#include <wincon.h>

#define KBUP 328
#define KBDOWN 336

/// Declaration des structures de données
typedef struct _Position
{
    int numeroLigne;
    int ordreLigne;
    int numeroPhrase;
    int ordrePhrase;
    char* memory;
    struct _Position* suivant;
    struct _Position* precTexte;
    struct _Position* suivTexte;
} T_Position;

typedef struct _Noeud
{
    char* mot;
    int nbOccurences;
    T_Position* listePositions;
    struct _Noeud* filsGauche;
    struct _Noeud* filsDroit;
    int hauteur;
} T_Noeud;

typedef struct _Index
{
    T_Noeud* racine;
    int nbMotsDistincts;
    int nbMotsTotal;
    int nbPoints;
    char *nomFichier;
    T_Position* debutTexte; // Pointeur vers le tout premier mot (Essentiel pour reconstruire)
} T_Index;

/// Déclaration du menu
void initialisation(char** LM);
int menu(char** tab, COORD O);

/// Fonctions utilitaires AVL
int maxi(int a, int b);
int hauteur(T_Noeud *N);
int getEquilibre(T_Noeud *N);
T_Noeud *rotationDroite(T_Noeud *y);
T_Noeud *rotationGauche(T_Noeud *x);

/// Fonctions Principales (Mises à jour avec dernierPos)

// Ajout de dernierPos pour le chainage
T_Noeud* insererAVL(T_Noeud* node, char* mot_traite, char* mot_init, int ligne, int ordreL, int phrase, int ordreP, int* estNouveau, T_Position** dernierPos);

// Ajout de dernierPos pour le chainage
T_Position* ajouterPosition(T_Position* listeP, char* mot, int ligne, int ordreL, int phrase, int ordreP, int *maj, T_Position** dernierPos);

// Ajout de dernierPos pour le chainage
int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordreL, int phrase, int ordreP, T_Position** dernierPos);

int indexerFichier(T_Index *index, char *filename);
T_Noeud* rechercherMot(T_Index* index, char *mot);
void construireTexteCouleur(T_Index index, const char* mot);

void afficherIndex(T_Index index);
void afficherArbreRec(T_Noeud *noeud, char *derniereLettre);

// Nouvelle version optimisée (sans argument Point)
void afficherOccurencesMot(T_Index* index, char* mot);

// Nouvelle version optimisée (sans argument listePoints)
void construireTexte(T_Index index, char *filename);



// --- Fonctions d'affichage Console Windows ---
void color(int t,int f);
void gotoxy(int x, int y);


#endif

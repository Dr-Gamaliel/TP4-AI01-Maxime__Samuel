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



/*
// Anciennes structures pour la gestion par Piles
typedef struct _celluleMot
{
    char* mot;
} celluleMot;

typedef struct _Pile
{
    celluleMot* tab;
    int* ligne;
    int sommet;
    int Longueur;
} Pile;

typedef struct _element
{
    int numphrase;
    Pile* phrase;
} Element;

typedef struct CellulePhrase {
    int numeroPhrase;
    // T_CelluleMot *listeMots; // Note : T_CelluleMot n'etait pas defini dans ton snippet original
    struct CellulePhrase *suivant;
} T_CellulePhrase;
*/

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



/*
// --- Fonctions de gestion des Piles ---
Pile* creer_pile(int l);
void supprimer_Pile(Pile* P);
int est_vide(Pile* P);
int est_pleine(Pile* P);
void empiler(Pile* P, char* mot, int ordre,int ligne);
char* depiler(Pile* P);
int dicho_search(Element* tab, int X, int debut, int fin);
void parcours_position(T_Position* X, Element* E, int taille_E);
void parcours_infixe(T_Noeud* X, Element* E, int taille_E);
*/

#endif

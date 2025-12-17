#include "tp4.h"

/// --- AFFICHAGE DE L'INDEX ---

void afficherArbreRec(T_Noeud *noeud, char *derniereLettre)
{
    if (noeud == NULL) return;

    afficherArbreRec(noeud->filsGauche, derniereLettre);

    char c = noeud->mot[0];
    if (c >= 'a' && c <= 'z') c -= 32;

    if (c != *derniereLettre) {
        if (*derniereLettre != '\0') printf("\n");
        printf("%c\n", c);
        *derniereLettre = c;
    }

    char motJoli[100];
    strcpy(motJoli, noeud->mot);
    if(motJoli[0] >= 'a' && motJoli[0] <= 'z') motJoli[0] -= 32;

    printf("|-- %s\n", motJoli);

    T_Position *p = noeud->listePositions;
    while (p != NULL) {
        printf("|---- (l:%d, o:%d, p:%d)\n", p->numeroLigne, p->ordreLigne, p->numeroPhrase);
        p = p->suivant;
    }
    printf("|\n");

    afficherArbreRec(noeud->filsDroit, derniereLettre);
}

void afficherIndex(T_Index index)
{
    char lettre = '\0';
    afficherArbreRec(index.racine, &lettre);
}
///affichage des occurences de chaque mot
//le principe est le suivant, on cherhce notre mot dans l'arbre,
//on crée un tableau de type elemet qui contient une pile, chaque liste représentant une phrase,
//ensuite on reparcours l'arbre en stoquant tous les mots dont le numPhrase est un elt->num du tableau, dans la case correspondante à sa phrase et à l'ordre correspondant à son ordre
//ce qui nous permet au pire des cas de faire un parcours de l'ordre de la taille du texte tout entier

/*
// on définit les fonctions qui permettront de gérer la file représentant une phrase
Pile* creer_pile(int l)
{
    Pile* P=malloc(sizeof(Pile));
    P->Longueur=l;
    P->sommet=-1;
    P->tab=malloc(sizeof(celluleMot)*l);
    for(int i=0; i<l; i++)
       {
           P->tab[i].mot=malloc(sizeof(char)*100);
       }
    P->ligne=malloc(sizeof(int)*10);
    for(int j=0; j<10; j++) P->ligne[j]=0;
    return(P);
}
void supprimer_Pile(Pile* P)
{
    free(P->tab);
    free(P);
}

int est_vide(Pile* P)
{
    if(P)
        return(P->sommet==-1);
    return(1);
}
int est_pleine(Pile* P)
{
    if(P)
        return(P->sommet==P->Longueur-1);
    return (1);
}

//on a même pas besoin de ranger la pile ou de trier les mots, on insère simplement chaque mot à l'indice longueurPhrase - ordre du mot dans la phrase+1
void empiler(Pile* P, char* mot, int ordre,int ligne)
{
    if(!est_pleine(P))
    {
        int newline=-1,controle=1;//comme la même phrase peut s'étendre sur plusieurs lignes, on ne va stoquer que les lignes différentes
         P->sommet++;
         strcpy(P->tab[P->Longueur-ordre].mot,mot);
         for(int i=0;i<10;i++)//comme 10=cste, O(1) pour cette boucle
         {
            if(P->ligne[i]==ligne)
              {
                   controle=0;
                   break;
              }
            if(P->ligne[i]!=0)
                newline++;
         }
         if(controle)
            P->ligne[newline+1]=ligne;
    }
}
char* depiler(Pile* P)
{
    if(!est_vide(P))
    {
        P->sommet--;
        return(P->tab[P->sommet+1].mot);
    }
    return(NULL);
}

// on fait une recherche dichotomique du numéro d'une phrase dans le tableau contenant les numéros de tables que nous sommes en train de construire
int dicho_search(Element* tab, int X, int debut, int fin)
{
    int g=debut, d=fin, i=(g+d)/2;
    if(tab && tab[g].numphrase<=X &&tab[d].numphrase>=X)
    {
        if(tab[i].numphrase==X)
            return(i);
        if(tab[g].numphrase==X)
            return(g);
        if(tab[d].numphrase==X)
            return(d);
        if(tab[i].numphrase<X)
        {
            g=i+1;
            return(dicho_search(tab,X,g,d));
        }
        if(tab[i].numphrase>X)
        {
            d=i-1;
            return(dicho_search(tab,X,g,d));
        }
    }
    return(-1);
}
//on définit une fonction pour parcourir la totalité d'une liste de position, du moins tant que les numéros de phrases peuvent correspondre
void parcours_position(T_Position* X, Element* E, int taille_E)
{
    if(X && E)
    {
        int est_present=dicho_search(E,X->numeroPhrase,0,taille_E);
        if(est_present!=-1)
            empiler(E[est_present].phrase,X->memory,X->ordrePhrase,X->numeroLigne);
        parcours_position(X->suivant,E,taille_E);
    }
}

// on définit une fonction pour parcourir la totalité de l'arbre
void parcours_infixe(T_Noeud* X, Element* E, int taille_E)
{
    if(X && E)
    {
        parcours_infixe(X->filsGauche, E, taille_E);
        parcours_position(X->listePositions,E, taille_E);
        parcours_infixe(X->filsDroit,E, taille_E);
    }
}
*/
void afficherOccurencesMot(T_Index* index, char* mot)
{
    // Recherche du noeud dans l'AVL
    T_Noeud* cible = rechercherMot(index, mot);
    
    if (!cible) {
        printf("Le mot \"%s\" est absent du texte.\n", mot);
        return;
    }

    printf("Mot = \"%s\"\nOccurences = %d\n", cible->mot, cible->nbOccurences);

    T_Position* p = cible->listePositions;
    
    // Pour chaque occurrence du mot trouvé
    while (p != NULL) {
        printf("\n| Ligne %d, Phrase %d : ", p->numeroLigne, p->numeroPhrase);

        // On remonte via precTexte tant qu'on est dans la même phrase
        T_Position* curseur = p;
        while (curseur->precTexte != NULL && curseur->precTexte->numeroPhrase == p->numeroPhrase) {
            curseur = curseur->precTexte;
        }

        // On affiche tant qu'on est dans la même phrase
        while (curseur != NULL && curseur->numeroPhrase == p->numeroPhrase) {
            

            if (strcmp(curseur->memory, "POINT") == 0) {
                printf(".");
            } else {
                printf("%s ", curseur->memory);
            }
            
            curseur = curseur->suivTexte;
        }
        
        // Passage à l'occurrence suivante du mot cible
        p = p->suivant;
    }
    printf("\n");
}




void construireTexte(T_Index index, char *filename) {
    if (index.debutTexte == NULL) {
        printf("L'index est vide ou debutTexte non initialise.\n");
        return;
    }

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Erreur d'ouverture fichier %s\n", filename);
        return;
    }

    // On part du tout premier mot du texte sauvegarde dans indexerfich
    T_Position* p = index.debutTexte;
    int ligneActuelle = p->numeroLigne;

    while (p != NULL) {
        
        // Gestion des sauts de ligne
        while (ligneActuelle < p->numeroLigne) {
            fprintf(fp, "\n");
            ligneActuelle++;
        }

        // Si c'est un POINT
        if (p->memory && strcmp(p->memory, "POINT") == 0) {
            fprintf(fp, ".");
        } 
        // Si c'est un MOT normal
        else if (p->memory) {
            fprintf(fp, "%s", p->memory);
            
            // On ajoute un espace après le mot SEULEMENT SI :
            if (p->suivTexte != NULL && 
                p->suivTexte->memory != NULL && 
                strcmp(p->suivTexte->memory, "POINT") != 0) {
                fprintf(fp, " ");
            }
        }

        // On suit le fil d'Ariane vers le mot suivant
        p = p->suivTexte;
    }

    fclose(fp);
    printf("Texte reconstruit avec succes dans '%s'.\n", filename);
}









/*

void insererMotTrie(T_CelluleMot **liste, char *mot, int ordre) {
    T_CelluleMot *nouveau = malloc(sizeof(T_CelluleMot));
    nouveau->mot = mot; // On pointe juste vers le mot de l'index (pas de copie nécessaire)
    nouveau->ordre = ordre;
    nouveau->suivant = NULL;

    if (*liste == NULL || (*liste)->ordre > ordre) {
        nouveau->suivant = *liste;
        *liste = nouveau;
    } else {
        T_CelluleMot *courant = *liste;
        while (courant->suivant != NULL && courant->suivant->ordre < ordre) {
            courant = courant->suivant;
        }
        nouveau->suivant = courant->suivant;
        courant->suivant = nouveau;
    }
}

// Trouve ou crée la phrase et y insère le mot
void ajouterAuTexteStruct(T_CellulePhrase **texte, int numPhrase, int ordre, char *mot) {
    T_CellulePhrase *courant = *texte;
    T_CellulePhrase *prec = NULL;

    // Recherche de la phrase ou du point d'insertion
    while (courant != NULL && courant->numeroPhrase < numPhrase) {
        prec = courant;
        courant = courant->suivant;
    }

    // Si la phrase n'existe pas, on la crée
    if (courant == NULL || courant->numeroPhrase != numPhrase) {
        T_CellulePhrase *nouvellePhrase = malloc(sizeof(T_CellulePhrase));
        nouvellePhrase->numeroPhrase = numPhrase;
        nouvellePhrase->listeMots = NULL;
        nouvellePhrase->suivant = courant;

        if (prec == NULL) *texte = nouvellePhrase; // Insertion en tête
        else prec->suivant = nouvellePhrase;       // Insertion milieu/fin

        courant = nouvellePhrase;
    }

    // On insère le mot dans la phrase trouvée/créée
    insererMotTrie(&(courant->listeMots), mot, ordre);
}

// Parcours récursif de l'index pour remplir la structure
void remplirStructureTexte(T_Noeud *noeud, T_CellulePhrase **texte) {
    if (noeud == NULL) return;

    // sous arbre g
    remplirStructureTexte(noeud->filsGauche, texte);

    // noeud courant
    T_Position *pos = noeud->listePositions;
    while (pos != NULL) {
        ajouterAuTexteStruct(texte, pos->numeroPhrase, pos->ordre, noeud->mot);
        pos = pos->suivant;
    }

    // sous arbre D
    remplirStructureTexte(noeud->filsDroit, texte);
}


void construireTexte(T_Index index, char *filename) {
    T_CellulePhrase *texteReconstruit = NULL;

    // 1. Remplissage de la structure temporaire
    // On transforme l'arbre (mots triés) en liste de phrases (phrases triées)
    remplirStructureTexte(index.racine, &texteReconstruit);

    // 2. Écriture dans le fichier
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Erreur d'ouverture du fichier de sortie.\n");
        return;
    }

    T_CellulePhrase *phraseCourante = texteReconstruit;
    while (phraseCourante != NULL) {
        T_CelluleMot *motCourant = phraseCourante->listeMots;

        while (motCourant != NULL) {
            fprintf(fp, "%s", motCourant->mot);
            // Ajout d'un espace si ce n'est pas le dernier mot
            if (motCourant->suivant != NULL) {
                fprintf(fp, " ");
            }
            motCourant = motCourant->suivant;
        }
        // Fin de phrase
        fprintf(fp, ".\n"); // On ajoute un point par défaut

        phraseCourante = phraseCourante->suivant;
    }

    fclose(fp);
    printf("Texte reconstruit dans %s\n", filename);

    // 3. Libération de la mémoire de la structure temporaire
    while (texteReconstruit != NULL) {
        T_CellulePhrase *pTmp = texteReconstruit;
        T_CelluleMot *mTmp = pTmp->listeMots;
        while (mTmp != NULL) {
            T_CelluleMot *mSuiv = mTmp->suivant;
            free(mTmp);
            mTmp = mSuiv;
        }
        texteReconstruit = texteReconstruit->suivant;
        free(pTmp);
    }
}

*/

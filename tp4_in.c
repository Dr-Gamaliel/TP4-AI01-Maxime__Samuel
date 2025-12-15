#include "tp4.h"

/// FONCTIONS POUR AVL

int maxi(int a, int b) {
    return (a > b) ? a : b;
}

int hauteur(T_Noeud *N) {
    if (N == NULL)
        return 0;
    return N->hauteur;
}

int getEquilibre(T_Noeud *N) {
    if (N == NULL)
        return 0;
    return hauteur(N->filsGauche) - hauteur(N->filsDroit);
}

// Rot droit
T_Noeud *rotationDroite(T_Noeud *y) {
    T_Noeud *x = y->filsGauche;
    T_Noeud *T2 = x->filsDroit;
    x->filsDroit = y;
    y->filsGauche = T2;
    //MAj des hauteurs
    y->hauteur = maxi(hauteur(y->filsGauche), hauteur(y->filsDroit)) + 1;
    x->hauteur = maxi(hauteur(x->filsGauche), hauteur(x->filsDroit)) + 1;

    return x;
}

// Rot gauche copier coller de rot droit juste inversion du sens de rot
T_Noeud *rotationGauche(T_Noeud *x) {
    T_Noeud *y = x->filsDroit;
    T_Noeud *T2 = y->filsGauche;
    y->filsGauche = x;
    x->filsDroit = T2;
    x->hauteur = maxi(hauteur(x->filsGauche), hauteur(x->filsDroit)) + 1;
    y->hauteur = maxi(hauteur(y->filsGauche), hauteur(y->filsDroit)) + 1;
    return y;
}

T_Noeud* insererAVL(T_Noeud* node, char* mot_traite, char* mot_init, int ligne, int ordreL, int phrase, int ordreP, int* estNouveau) {
    int* maj= malloc(sizeof(int));
    *maj=0;
    if (node == NULL) {
        T_Noeud* nouveau = malloc(sizeof(T_Noeud));
        nouveau->mot = malloc(strlen(mot_traite) + 1);
        strcpy(nouveau->mot, mot_traite);
        nouveau->nbOccurences = 1;
        nouveau->listePositions = NULL;
        nouveau->listePositions = ajouterPosition(nouveau->listePositions, mot_init,ligne, ordreL, phrase, ordreP,maj);
        nouveau->filsGauche = NULL;
        nouveau->filsDroit = NULL;
        nouveau->hauteur = 1;
        *estNouveau = 1;
        return nouveau;
    }


    // Savoir ou j'insere avec le strcmp
    int cmp = strcmp(mot_traite, node->mot);

    // mot qui va a gauche
    if (cmp < 0)
        node->filsGauche = insererAVL(node->filsGauche, mot_traite, mot_init, ligne, ordreL, phrase, ordreP, estNouveau);
    // mot qui va a droite
    else if (cmp > 0)
        node->filsDroit = insererAVL(node->filsDroit, mot_traite, mot_init, ligne, ordreL, phrase, ordreP, estNouveau);
    // mot qui existe
    else {
        node->listePositions = ajouterPosition(node->listePositions, mot_init, ligne, ordreL, phrase, ordreP,maj);
        if(*maj)
            node->nbOccurences++;
        *estNouveau = 0;
        return node;
    }

    // mise à jour de la hauteur du noeud ancêtre
    node->hauteur = 1 + maxi(hauteur(node->filsGauche), hauteur(node->filsDroit));

    //  equilibre
    int balance = getEquilibre(node);

    // ICI GESTION DE L'EQUILIBRE

    // Cas Gauche-Gauche
    if (balance > 1 && strcmp(mot_traite, node->filsGauche->mot) < 0)
        return rotationDroite(node);

    // Cas Droite-Droite
    if (balance < -1 && strcmp(mot_traite, node->filsDroit->mot) > 0)
        return rotationGauche(node);

    // Cas Gauche-Droite
    if (balance > 1 && strcmp(mot_traite, node->filsGauche->mot) > 0) {
        node->filsGauche = rotationGauche(node->filsGauche);
        return rotationDroite(node);
    }

    // Cas Droite-Gauche
    if (balance < -1 && strcmp(mot_traite, node->filsDroit->mot) < 0) {
        node->filsDroit = rotationDroite(node->filsDroit);
        return rotationGauche(node);
    }
    free(maj);
    return node;
}


/// FONCTIONS DU TP
T_Position* ajouterPosition(T_Position* listeP, char* mot,int ligne, int ordreL, int phrase, int ordreP, int *maj)
{
    T_Position* x, *inserable;
    x=listeP;
    inserable=malloc(sizeof(T_Position));
    inserable->memory=malloc(sizeof(char)*100);
    strcpy(inserable->memory,mot);
    inserable->numeroLigne=ligne;
    inserable->numeroPhrase=phrase;
    inserable->ordreLigne=ordreL;
    inserable->ordrePhrase=ordreP;
    inserable->suivant=NULL;
    if(!x)
    {
        x=inserable;
        *maj=1;
    }
    else
    {
        // J'ai modifié le && pour un ||
        if(x->numeroLigne > ligne || (x->numeroLigne == ligne && x->ordreLigne > ordreL))
        {
            inserable->suivant=x;
            x=inserable;
            *maj=1;
        }
        else
        {
            while(x->suivant!=NULL)
            {
                // pareil ici
                if(x->suivant->numeroLigne > ligne || (x->suivant->numeroLigne == ligne && x->suivant->ordreLigne > ordreL))
                {
                    break;
                }
                x=x->suivant;
            }
            if( x->ordreLigne != ordreL)
            {
                inserable->suivant=x->suivant;
                x->suivant=inserable;
                *maj=1;
            }
            x=listeP;// je remets x au début pour ne pas perdre la tête
        }
    }
    return(x);
}

int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordreL, int phrase, int ordreP) {
    if (index == NULL || mot == NULL) return 0;

    // Copie et mise en minuscule
    char motTraite[100];
    int i = 0;
    while(mot[i] != '\0') {
        if(mot[i] >= 'A' && mot[i] <= 'Z')
            motTraite[i] = mot[i] + 32;
        else
            motTraite[i] = mot[i];
        i++;
    }//O(1)
    motTraite[i] = '\0';

    int estNouveau = 0;

    // Si l'arbre est vide, on initialise le compteur total si besoin
    if (index->racine == NULL) {
        index->nbMotsDistincts = 0;
        index->nbMotsTotal = 0;
    }

    // Appel de la fonction AVL récursive
    // Notez que index->racine peut changer si la racine subit une rotation
    index->racine = insererAVL(index->racine, motTraite, mot, ligne, ordreL, phrase, ordreP, &estNouveau);

    if (estNouveau) {
        index->nbMotsDistincts++;
    }
    index->nbMotsTotal++;

    return 1;
}

int indexerFichier(T_Index *index,T_Position** Point, char *filename) {
    index->nomFichier = filename; // Sauvegarde du nom pour l'affichage plus tard
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        return 0;
    }
    int nbMotsTotal = 0;
    int ligne = 1;
    int ordreL = 1;
    int phrase = 1;
    int ordreP = 1;

    char motBuffer[100];
    int i = 0;
    char c;

    while ((c = fgetc(fp)) != EOF) {
        int maj=0;
        // Si c'est une lettre (majuscule ou minuscule)
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            motBuffer[i] = c;
            i++;
        }
        else {
            // C'est un séparateur (espace, ponctuation, retour ligne...)
            // 1. Si on avait un mot en cours, on l'enregistre
            if (i > 0) {
                motBuffer[i] = '\0';
                ajouterOccurence(index, motBuffer, ligne, ordreL, phrase, ordreP);
                nbMotsTotal++;
                ordreL++;
                ordreP++;
                i = 0;
            }

            // 2. Gestion spécifique des séparateurs
            if (c == '\n') {
                ligne++;
                ordreL = 1;
            }
            // MODIFICATION ICI : On ne gère QUE le point '.'
            else if (c == '.') {
                *Point=ajouterPosition(*Point,"POINT",ligne, ordreL, phrase,ordreP,&maj);
                if(maj)
                    index->nbPoints++;
                phrase++;
                ordreP = 1;
            }
        }
    }

    // Traitement du tout dernier mot si le fichier ne finit pas par un séparateur
    if (i > 0) {
        motBuffer[i] = '\0';
        ajouterOccurence(index, motBuffer, ligne, ordreL, phrase, ordreP);
        nbMotsTotal++;
    }

    fclose(fp);
    return nbMotsTotal;
}



// Samuel
// j'ai ajouté le fais de mettre tt les moots en minuscules pour pouvoir les comparé dans l'arbre sinon probleme lors de la recherche
T_Noeud* rechercherMot(T_Index* index, char *mot)
{
    T_Noeud* X = NULL;
    if(index && mot)
    {
        char motCherche[100];
        int i = 0;

        while(mot[i] != '\0' && mot[i] != '\n')
        {
            if(mot[i] >= 'A' && mot[i] <= 'Z')
                motCherche[i] = mot[i] + 32;
            else
                motCherche[i] = mot[i];
            i++;
        }
        motCherche[i] = '\0';

        X = index->racine;
        int trouve = 0;

        while(!trouve && X)
        {
            int pos = strcmp(motCherche, X->mot);

            if(pos == 0) { trouve = 1; }
            else if(pos < 0) { X = X->filsGauche; }
            else { X = X->filsDroit; }
        }
    }
    return(X);
}

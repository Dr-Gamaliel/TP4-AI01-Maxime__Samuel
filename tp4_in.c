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

T_Noeud* insererAVL(T_Noeud* node, char* mot_traite, char* mot_init, int ligne, int ordreL, int phrase, int ordreP, int* estNouveau, T_Position** dernierPos) {
    int maj = 0; 

    if (node == NULL) {
        T_Noeud* nouveau = malloc(sizeof(T_Noeud));
        if (nouveau == NULL) return NULL; 

        nouveau->mot = malloc(strlen(mot_traite) + 1);
        strcpy(nouveau->mot, mot_traite);
        
        nouveau->nbOccurences = 1;
        nouveau->listePositions = NULL;
        
        nouveau->listePositions = ajouterPosition(nouveau->listePositions, mot_init, ligne, ordreL, phrase, ordreP, &maj, dernierPos);
        
        nouveau->filsGauche = NULL;
        nouveau->filsDroit = NULL;
        nouveau->hauteur = 1;
        
        *estNouveau = 1;
        return nouveau;
    }

    // Recherche de la position d'insertion
    int cmp = strcmp(mot_traite, node->mot);

    // On passe dernierPos récursivement
    if (cmp < 0) {
        node->filsGauche = insererAVL(node->filsGauche, mot_traite, mot_init, ligne, ordreL, phrase, ordreP, estNouveau, dernierPos);
    }
    else if (cmp > 0) {
        node->filsDroit = insererAVL(node->filsDroit, mot_traite, mot_init, ligne, ordreL, phrase, ordreP, estNouveau, dernierPos);
    }
    else {
        // Le mot existe déjà, on ajoute la position (avec dernierPos)
        node->listePositions = ajouterPosition(node->listePositions, mot_init, ligne, ordreL, phrase, ordreP, &maj, dernierPos);
        
        if(maj)
            node->nbOccurences++;
            
        *estNouveau = 0;
        return node;
    }

    // Mise à jour hauteur et équilibrage 
    node->hauteur = 1 + maxi(hauteur(node->filsGauche), hauteur(node->filsDroit));
    int balance = getEquilibre(node);

    if (balance > 1 && strcmp(mot_traite, node->filsGauche->mot) < 0)
        return rotationDroite(node);

    if (balance < -1 && strcmp(mot_traite, node->filsDroit->mot) > 0)
        return rotationGauche(node);

    if (balance > 1 && strcmp(mot_traite, node->filsGauche->mot) > 0) {
        node->filsGauche = rotationGauche(node->filsGauche);
        return rotationDroite(node);
    }

    if (balance < -1 && strcmp(mot_traite, node->filsDroit->mot) < 0) {
        node->filsDroit = rotationDroite(node->filsDroit);
        return rotationGauche(node);
    }

    return node;
}

/// FONCTIONS DU TP
T_Position* ajouterPosition(T_Position* listeP, char* mot, int ligne, int ordreL, int phrase, int ordreP, int *maj, T_Position** dernierPos)
{
    T_Position* x = listeP;
    T_Position* inserable = malloc(sizeof(T_Position));
    
    if (mot) {
        inserable->memory = malloc(strlen(mot) + 1);
        strcpy(inserable->memory, mot);
    } else {
        inserable->memory = NULL;
    }

    inserable->numeroLigne = ligne;
    inserable->numeroPhrase = phrase;
    inserable->ordreLigne = ordreL;
    inserable->ordrePhrase = ordreP;
    inserable->suivant = NULL;

    inserable->precTexte = *dernierPos; 
    inserable->suivTexte = NULL;        

    if (*dernierPos != NULL) {
        (*dernierPos)->suivTexte = inserable;
    }
    *dernierPos = inserable; 

    if(!x)
    {
        x = inserable;
        *maj = 1;
    }
    else
    {
        if(x->numeroLigne > ligne || (x->numeroLigne == ligne && x->ordreLigne > ordreL))
        {
            inserable->suivant = x;
            x = inserable;
            *maj = 1;
        }
        else
        {
            T_Position* tete = x;
            while(x->suivant != NULL)
            {
                if(x->suivant->numeroLigne > ligne || (x->suivant->numeroLigne == ligne && x->suivant->ordreLigne > ordreL))
                {
                    break;
                }
                x = x->suivant;
            }
            // On vérifie qu'on n'insère pas exactement la même position (doublon strict)
            if(x->ordreLigne != ordreL || x->numeroLigne != ligne)
            {
                inserable->suivant = x->suivant;
                x->suivant = inserable;
                *maj = 1;
            }
            x = tete; 
        }
    }
    return(x);
}

int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordreL, int phrase, int ordreP, T_Position** dernierPos) {
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
    }
    motTraite[i] = '\0';

    int estNouveau = 0;

    if (index->racine == NULL) {
        index->nbMotsDistincts = 0;
        index->nbMotsTotal = 0;
    }

    index->racine = insererAVL(index->racine, motTraite, mot, ligne, ordreL, phrase, ordreP, &estNouveau, dernierPos);

    if (estNouveau) {
        index->nbMotsDistincts++;
    }
    index->nbMotsTotal++;

    return 1;
}
int indexerFichier(T_Index *index, char *filename) {
    index->nomFichier = filename;
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

    // Fil d'Ariane local
    T_Position* dernierPosGlobal = NULL; 
    index->debutTexte = NULL; 

    while ((c = fgetc(fp)) != EOF) {
        // ... (ton code de gestion des caractères ne change pas) ...
        
        // On accepte lettres, chiffres et tirets
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-') { 
            motBuffer[i] = c;
            i++;
        }
        else {
            // Fin d'un mot
            if (i > 0) {
                motBuffer[i] = '\0';
                
                // Ajout classique
                ajouterOccurence(index, motBuffer, ligne, ordreL, phrase, ordreP, &dernierPosGlobal);
                
                // Capture du début du texte
                if (index->debutTexte == NULL) {
                    index->debutTexte = dernierPosGlobal;
                }

                nbMotsTotal++;
                ordreL++;
                ordreP++;
                i = 0;
            }

            
            if (c == '\n') {
                ligne++;
                ordreL = 1;
            }
            
            else if (c == '.') {
                ajouterOccurence(index, "POINT", ligne, ordreL, phrase, ordreP, &dernierPosGlobal);
                
                // Sécurité début de texte (si le texte commence par un point...)
                if (index->debutTexte == NULL) {
                    index->debutTexte = dernierPosGlobal;
                }
                
                index->nbPoints++;
                
                phrase++;
                ordreP = 1;
            }
            // ----------------------
        }
    }

    if (i > 0) {
        motBuffer[i] = '\0';
        ajouterOccurence(index, motBuffer, ligne, ordreL, phrase, ordreP, &dernierPosGlobal);
        if (index->debutTexte == NULL) {
            index->debutTexte = dernierPosGlobal;
        }
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

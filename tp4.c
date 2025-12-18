#include "tp4.h"

/// FONCTIONS DU TP
//ajouter position
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
//ajouter occurence
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
//indexer fichier
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
//rechercher un mot
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
//afficher l'index
void afficherIndex(T_Index index)
{
    char lettre = '\0';
    afficherArbreRec(index.racine, &lettre);
}
//afficher les occurences d'un mot
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
                if(strcmp(curseur->suivTexte->memory, "POINT") == 0)
                    printf("%s", curseur->memory);
                else
                   printf("%s ", curseur->memory);
            }

            curseur = curseur->suivTexte;
        }

        // Passage à l'occurrence suivante du mot cible
        p = p->suivant;
    }
    printf("\n");
}
//construire le texte
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
            fprintf(fp, ". ");
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

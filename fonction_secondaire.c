#include "tp4.h"


/// fonction pour le menu

void color(int t,int f)
{
        HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(H,f*16+t);
}

void gotoxy(int x, int y)
{
   HANDLE H;
   COORD O;
   O.X = x;
   O.Y = y;
   H= GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleCursorPosition(H,O);
}

void initialisation(char** LM)
{
    for(int i=0; i<9; i++)
        LM[i]=malloc(sizeof **LM *50);
    strcpy(LM[0],"             ----menu----                ");
    strcpy(LM[1],"Charger un fichier");
    strcpy(LM[2],"Caracteristiques index");
    strcpy(LM[3],"Afficher index");
    strcpy(LM[4],"Rechercher un mot");
    strcpy(LM[5],"Afficher les occurences d'un mot");
    strcpy(LM[6],"Construire le texte a partir de l'index");
    strcpy(LM[7],"Quitter");
    strcpy(LM[8],"Cette partie sera disponible bientot");
}

int menu( char** tab, COORD O)
{
    system("cls");
    O.Y=+2;
    int choix=1, k=0;
    do
    {
        int y=O.Y;
        color(15,0);
        gotoxy(O.X,O.Y-2);
        printf("\n %s\n",tab[0]);
        for (int i=1; i<8; i++)
        {
            if (i==choix)
            {
                     gotoxy(O.X,y++);
                     color(0,15);
                     printf("%d. %s",i,tab[i]);
                     for(int j=strlen(tab[i]); j<strlen(tab[6]); j++) printf(" ");
                     printf("\n");
            }
            else
            {
                gotoxy(O.X,y++);
                color(15,0);
                printf("%d. %s",i,tab[i]);
                for(int j=strlen(tab[i]); j<strlen(tab[6]); j++) printf(" ");
                printf("\n");
            }
        }
        k=getch();
        if (k<=55&&k>=49)
        {
            choix=k%49+1;
            k=13;
        }
        else if(k!=13)
        {
            k=getch();
            if (k+256==KBDOWN)
            {
                choix++;
                if(choix==8) choix=1;
            }
            if (k+256==KBUP)
            {
                choix--;
                if(choix==0) choix=7;
            }
        }
    }while(k!=13);
    return(choix);
}

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

///fonction supplémentaire pour afficher le mot trouve en couleur dans le texte

void construireTexteCouleur(T_Index index, const char* mot)
{
    printf("\n\nVoici, dans le texte, les positions de \"%s\"\n\n",mot);
    // On part du tout premier mot du texte sauvegarde dans indexerfich
    T_Position* p = index.debutTexte;
    int ligneActuelle = p->numeroLigne;
    while (p != NULL) {

        // Gestion des sauts de ligne
        while (ligneActuelle < p->numeroLigne) {
            printf("\n");
            ligneActuelle++;
        }

        // Si c'est un POINT
        if (p->memory && strcmp(p->memory, "POINT") == 0)
        {
            if(strcmp(mot,"POINT")==0)
            {
                color(12,0);
                printf(".");
                color(15,0);
                printf(" ");
            }
            else
                printf(". ");
        }
        // Si c'est un MOT normal
        else if (p->memory)
        {
            char mottraite[100];
            mottraite[99]='\0';
            char c; int taille=strlen(p->memory);
            for(int i=0; i<taille; i++)
            {
                c=p->memory[i];
                if(c>='A'&&c<='Z') c+=32;
                mottraite[i]=c;
            }
            mottraite[taille]='\0';

            //si ce n'est pas le mot recherche
            if (strcmp(mottraite, mot) != 0)
                printf("%s", p->memory);
            //si c'est le mot recherche
            else
            {
                color(12,0);
                printf("%s", p->memory);
                color(15,0);
            }
            // On ajoute un espace après le mot SEULEMENT SI :
            if (p->suivTexte != NULL && p->suivTexte->memory != NULL && strcmp(p->suivTexte->memory, "POINT") != 0)
            {
                printf(" ");
            }
        }

        // On suit le fil d'Ariane vers le mot suivant
        p = p->suivTexte;
    }
 printf("\n\n");
}

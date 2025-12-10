#include "tp4.h"
/// mise en place du menu
/*void color(int t,int f)
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
}*/

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

int menu( char** tab)//, COORD O)
{
    //system("cls");
    //O.Y=+2;
    int choix=1, k=0;
    do
    {
        /*int y=O.Y;
        color(15,0);
        gotoxy(O.X,O.Y-2);*/
        printf("\n %s\n",tab[0]);
        for (int i=1; i<8; i++)
        {
            if (i==choix)
            {
                     /*gotoxy(O.X,y++);
                     color(0,15);*/
                     printf("%d. %s",i,tab[i]);
                     for(int j=strlen(tab[i]); j<strlen(tab[6]); j++) printf(" ");
                     printf("\n");
            }
            else
            {
                /*gotoxy(O.X,y++);
                color(15,0);*/
                printf("%d. %s",i,tab[i]);
                for(int j=strlen(tab[i]); j<strlen(tab[6]); j++) printf(" ");
                printf("\n");
            }
        }
        /*k=*/scanf("%d",&k);//getch();
        if (k<=7&&k>=1)//(k<=55&&k>=49)
        {
            choix=k;//%49+1;
            k=13;
        }
        else {choix=1; k=13;}
       /* else if(k!=13)
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
        }*/
    }while(k!=13);
    return(choix);
}
/// mise en place des fonctions de base
T_Position* ajouterPosition(T_Position* listeP, int ligne, int ordre, int phrase)
{
    T_Position* x, *inserable;
    x=listeP;
    inserable=malloc(sizeof(T_Position));
    inserable->numeroLigne=ligne;
    inserable->numeroPhrase=phrase;
    inserable->ordre=ordre;
    inserable->suivant=NULL;
    if(!x)
    {
        x=inserable;
    }
    else
    {
        if(x->numeroLigne>=ligne && x->ordre>ordre)
        {
            inserable->suivant=x;
            x=inserable;
        }
        else
        {
            while(x->suivant!=NULL)
            {
                if(x->suivant->numeroLigne>=ligne && x->suivant->ordre>ordre)
                {
                    break;
                }
                x=x->suivant;
            }
            inserable->suivant=x->suivant;
            x->suivant=inserable;
            x=listeP;
        }
    }
    return(x);
}

int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordre, int phrase) ///ne pas oublier de considerer le point
{
    if (index == NULL || mot == NULL) return 0;

    // Création de motTraite un copie du mot en cours tt en minuscule pour pouvoire faciliter le traitement
    char motTraite[100];
    int i = 0;
    while(mot[i] != '\0')
    {
        if(mot[i] >= 'A' && mot[i] <= 'Z')
        {
            motTraite[i] = mot[i] + 32; // passage maj vers min
        }
        else
        {
            motTraite[i] = mot[i];
        }
        i++;
    }
    motTraite[i] = '\0'; // fin de chaine


    // Si arbre vide je met mon noeud à la racine
    if (index->racine == NULL)
    {
        // implementation de la struct du noeud
        T_Noeud *nouveau = malloc(sizeof(T_Noeud));
        if (nouveau == NULL) return 0;

        nouveau->mot = malloc(strlen(motTraite) + 1);
        strcpy(nouveau->mot, motTraite);

        nouveau->nbOccurences = 1;
        nouveau-> filsGauche = NULL;
        nouveau-> filsDroit = NULL ;
        nouveau->listePositions = NULL ;
        // Appele de la fonction de Samuel pour ajt la position
        nouveau->listePositions = ajouterPosition(nouveau->listePositions, ligne, ordre, phrase);

        // J e met le noeud à la racine de l'index
        index->racine = nouveau;
        index->nbMotsDistincts = 1;
        index->nbMotsTotal = 1;
        return 1;
    }


    // Cas où l'arbre existe déja, recherche de où on met le noeud

    T_Noeud *courant = index->racine;
    while (1)
    {
        int comparaison = strcmp(motTraite, courant->mot);
        // Si mot identique
        if (comparaison == 0)
        {
            courant->listePositions = ajouterPosition(courant->listePositions, ligne, ordre, phrase);
            courant->nbOccurences++;
            index->nbMotsTotal++; // On a un mot de plus au total
            return 1;
        }
        // Cas 1 si mot plus petit alors je vais dans la partie gauche de l'arbre
        else if (comparaison < 0)
        {
        // si sous arbre gauche vide on met le mot ici
            if (courant->filsGauche == NULL)
            {
                T_Noeud *nouveau = malloc(sizeof(T_Noeud));
                nouveau->mot = malloc(strlen(motTraite) + 1);
                strcpy(nouveau->mot, motTraite);
                nouveau->nbOccurences = 1;
                nouveau->filsGauche = NULL;
                nouveau->filsDroit = NULL;
                nouveau->listePositions = NULL;
                nouveau->listePositions = ajouterPosition(nouveau->listePositions, ligne, ordre, phrase);
                courant->filsGauche = nouveau; // On attache le nouveau noeud
                index->nbMotsDistincts++;
                index->nbMotsTotal++;
                return 1;
            }
        // Sinon on continue de descendre à gauche jusqua nuull
        courant = courant->filsGauche;
        }

        // comparaison > 0 le mot est plus grand lexicographiquement donc je vais à droite
        // Similaire à precedent
        else
        {
            if (courant->filsDroit == NULL)
            {
                T_Noeud *nouveau = malloc(sizeof(T_Noeud));
                nouveau->mot = malloc(strlen(motTraite) + 1);
                strcpy(nouveau->mot, motTraite);
                nouveau->nbOccurences = 1;
                nouveau->filsGauche = NULL;
                nouveau->filsDroit = NULL;
                nouveau->listePositions = NULL;
                nouveau->listePositions = ajouterPosition(nouveau->listePositions, ligne, ordre, phrase);
                courant->filsDroit = nouveau; // On attache le nouveau noeud
                index->nbMotsDistincts++;
                index->nbMotsTotal++;
                return 1;
            }
            courant = courant->filsDroit;
        }
    }
}

/*
chien[(1,1,4)] chat[(2,1,4)] droit[(1,1,4);(2,2,3);(3,3,1)] bateau[(1,1,4)] cheval(3,3,3) avion(2,2,3) besoin(1,1,2) chef(8,2,3) chevre(3,1,3) chatte(1,1,3) dortoir(1,1,3) sorcier(1,6,3)
*/

void test()
{
    T_Index* index;
    T_Noeud* x;
    T_Position* y;
    index=malloc(sizeof(T_Index));
    for(int i=0; i<12; i++)
    {
        printf("resultat de l'ajour%d: %d\n",i+1,ajouterOccurence(index,"chien",1,4,1));
    }
    x=index->racine;i=1;
    while(x!=NULL)
    {
        printf("l'element %s de l'index, apparait %d fois",x->mot, x->nbOccurences);
        y=x->listePositions;
        int i=1;
        while(y!=NULL)
        {
            printf("fois %d:dans la ligne %d a l'ordre %d et dans la phrase %d\n",i,y->numeroLigne,y->ordre,y->numeroPhrase);
            i++;
            y=y->suivant
        }
        x=x->filsGauche;
    }
}
